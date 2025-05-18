#include "gameclient.h"


GameClient::GameClient(QObject *parent) :
    QObject(parent),
    socket(new QTcpSocket(this)),
    udpSocket(new QUdpSocket(this)),
    startServer(true),
    BadDisconected(true) //the indicator shows whether the player disconnected by choice or due to a connection failure
{
    connect(socket, &QTcpSocket::disconnected, this, [this] {
        bool wasBadDisconected = BadDisconected;
        emit disconnected();
        if (wasBadDisconected) {
            emit showMainWindow();
        }
    });
}

void GameClient::listenForBroadcasts() {
    udpSocket->bind(UDP_BROADCAST_PORT, QUdpSocket::ShareAddress);

    //as soon as the UDP socket receives a broadcast message, extract server data from it and send a signal to connect
    connect(udpSocket, &QUdpSocket::readyRead, this, &GameClient::handleBroadcastDatagram, Qt::UniqueConnection);
}

void GameClient::handleBroadcastDatagram() {
    if (!startServer) {
        return;
    }
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        if (datagram.startsWith("GameServer:")) {
            quint16 serverPort = datagram.mid(11).toUInt();
            emit serverFound(senderAddress.toString(), serverPort);
            startServer = false;
            udpSocket->close();
            break;
        }
    }
}

void GameClient::connectToServer(const QString& address, quint16 port) {
    socket->connectToHost(address, port);
    connect(socket, &QTcpSocket::readyRead, this, &GameClient::handleServerMessage, Qt::UniqueConnection);
}

void GameClient::handleServerMessage() {
    QByteArray data = socket->readAll();
    QString msg = QString::fromUtf8(data);

    //depending on the message received from the server, open the required window or send the reqired message to another player
    if (msg.contains(CHOOSE_OPTION_MSG)) {
        emit showChoicesWindow();
    }
    else if (msg.contains("You chose")) {
        emit showResultWindow(msg);
    }
    else if (msg.contains(PLAYER_LEFT_MSG)) {
        BadDisconected = false;
        emit showMainWindow();
    }
    else if (msg.contains(OTHER_PLAYER_LEFT_MSG)) {
        BadDisconected = false;
        emit showMessage("Player left the game");
    }
    else if (msg.contains(RESTART_REQUEST_MSG)) {
        emit showMessage("Player has sent a restart request");
    }
}

void GameClient::resetServer() {
    startServer = true;
}

void GameClient::resetConnectionFlag() {
    BadDisconected = true;
}

bool GameClient::serverStart() {
    return startServer;
}

void GameClient::sendChoice(const QString &choice) {
    qint64 bytesWritten = socket->write(choice.toUtf8());
    if (choice == QUIT_MSG && (bytesWritten == -1 || !socket->waitForBytesWritten(SOCKET_WAIT_TIMEOUT_MS))) {
        emit showMainWindow();
    }
}

bool GameClient::badDisconected() {
    return BadDisconected;
}

