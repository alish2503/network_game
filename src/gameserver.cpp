#include "gameserver.h"

GameServer::GameServer(QObject *parent) :
    QObject(parent),
    timer(new QTimer(this)),
    clientCount(0),
    server(new QTcpServer(this)),
    udpSocket(new QUdpSocket(this))
{
    connect(server, &QTcpServer::newConnection, this, &GameServer::onNewConnection);
}

void GameServer::start() {
    if (server->listen(QHostAddress::Any, 0)) {

        //sends a broadcast message at the specified interval
        connect(timer, &QTimer::timeout, this, &GameServer::broadcastServerInfo, Qt::UniqueConnection);
        timer->start(UDP_BROADCAST_INTERVAL_MS);
    }
}

void GameServer::broadcastServerInfo() {
    QByteArray message = "GameServer:" + QByteArray::number(server->serverPort());
    udpSocket->writeDatagram(message, QHostAddress::Broadcast, UDP_BROADCAST_PORT);
}


void GameServer::onNewConnection() {
    QTcpSocket *clientSocket = server->nextPendingConnection();
    emit clientConnected();

    //correctly disconnect a player if no one connected to him and he left
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::handleClientData);

    //correctly disconnect both players if at least one of them has a connection failure
    connect(clientSocket, &QTcpSocket::disconnected, this, &GameServer::onClientDisconnected, Qt::UniqueConnection);

    clients[clientCount++] = clientSocket;
    if (clientCount == MAX_CLIENTS) {
        //if the number of clients in the client array reaches two, then stop broadcasting the server
        timer->stop();
        startGame(clients[0], clients[1]);
    }
}

void GameServer::handleClientData() {
    if(clientCount < MAX_CLIENTS) {

        //if there is only one client, stop broadcasting the server
        timer->stop();
        clients[0]->write(PLAYER_LEFT_MSG);
        clients[0]->flush();
        clients[0]->disconnectFromHost();
    }
}

void GameServer::onClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(clientSocket && (!playerChoices.contains(clientSocket) || playerChoices[clientSocket] != QUIT_MSG)) {
        closeServer();
    }
}

void GameServer::startGame(QTcpSocket *player1, QTcpSocket *player2) {
    player1->write(CHOOSE_OPTION_MSG);
    player2->write(CHOOSE_OPTION_MSG);

    //start the game and process each player's choice
    connect(player1, &QTcpSocket::readyRead, this, &GameServer::onPlayerReady, Qt::UniqueConnection);
    connect(player2, &QTcpSocket::readyRead, this, &GameServer::onPlayerReady, Qt::UniqueConnection);
}

void GameServer::onPlayerReady() {
    QTcpSocket *player = qobject_cast<QTcpSocket*>(sender());
    if (!player) return;

    QTcpSocket *otherPlayer = (clients[0] == player) ? clients[1] : clients[0];
    handlePlayerChoice(player, otherPlayer);
}

void GameServer::handlePlayerChoice(QTcpSocket *currentPlayer, QTcpSocket *otherPlayer) {
    QByteArray data = currentPlayer->readAll();
    QString choice = QString::fromUtf8(data).trimmed();
    playerChoices[currentPlayer] = choice;

    //if a player chooses to restart, the server sends this request to another player, if both players choose to restart, we start the game again
    if (choice == RESTART_MSG) {
        if (otherPlayer->state() == QAbstractSocket::ConnectedState && playerChoices[otherPlayer] != RESTART_MSG) {
            otherPlayer->write(RESTART_REQUEST_MSG);
        }
        if (playerChoices[currentPlayer] == RESTART_MSG && playerChoices[otherPlayer] == RESTART_MSG) {
            playerChoices.clear();
            currentPlayer->write(CHOOSE_OPTION_MSG);
            otherPlayer->write(CHOOSE_OPTION_MSG);
        }
    }

    //if at least one player chooses to leave, this request is sent to the other player and the server disconnects both of them
    else if (choice == QUIT_MSG) {
        currentPlayer->write(PLAYER_LEFT_MSG);
        currentPlayer->flush();
        if (otherPlayer->state() == QAbstractSocket::ConnectedState) {
            otherPlayer->write(OTHER_PLAYER_LEFT_MSG);
        }
        currentPlayer->disconnectFromHost();
        if (playerChoices[currentPlayer] == QUIT_MSG && playerChoices[otherPlayer] == QUIT_MSG) {
            closeServer();
        }
    }
    //if both players make a move, we determine the winner
    else if (playerChoices.contains(currentPlayer) && playerChoices.contains(otherPlayer)) {
        QString player1Choice = playerChoices[currentPlayer];
        QString player2Choice = playerChoices[otherPlayer];
        QString message = "You chose: %1. Another player chose: %2. ";
        if (player1Choice == player2Choice) {
            currentPlayer->write((message+"Draw").arg(player1Choice, player2Choice).toUtf8());
            otherPlayer->write((message+"Draw").arg(player2Choice, player1Choice).toUtf8());
        } else if ((player1Choice == "Rock" && player2Choice == "Scissors") ||
                   (player1Choice == "Scissors" && player2Choice == "Paper") ||
                   (player1Choice == "Paper" && player2Choice == "Rock")) {
            currentPlayer->write((message+"You win").arg(player1Choice, player2Choice).toUtf8());
            otherPlayer->write((message+"You lose").arg(player2Choice, player1Choice).toUtf8());
        } else {
            currentPlayer->write((message+"You lose").arg(player1Choice, player2Choice).toUtf8());
            otherPlayer->write((message+"You win").arg(player2Choice, player1Choice).toUtf8());
        }
    }
}

void GameServer::closeServer() {
    if (clients[0] && clients[0]->state() == QAbstractSocket::ConnectedState) {
        clients[0]->disconnectFromHost();
    }
    if (clients[1] && clients[1]->state() == QAbstractSocket::ConnectedState) {
        clients[1]->disconnectFromHost();
    }
    playerChoices.clear();
    clients = {nullptr, nullptr};
    clientCount = 0;
    server->close();
}





