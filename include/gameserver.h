#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include "GameConstants.h"

class GameServer : public QObject {
    Q_OBJECT

public:
    GameServer(QObject *parent = nullptr);

    void start();
    void closeServer();

private:
    void handlePlayerChoice(QTcpSocket *currentPlayer, QTcpSocket *otherPlayer);
    void startGame(QTcpSocket *player1, QTcpSocket *player2);

signals:
    void clientConnected();

private slots:
    void onNewConnection();
    void broadcastServerInfo();
    void handleClientData();
    void onClientDisconnected();
    void onPlayerReady();

private:
    QTimer *timer;
    size_t clientCount;
    QTcpServer *server;
    QUdpSocket *udpSocket;
    std::array<QTcpSocket*, MAX_CLIENTS> clients = {nullptr, nullptr};
    QMap<QTcpSocket*, QString> playerChoices;

};

#endif
