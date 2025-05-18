#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include "GameConstants.h"

class GameClient : public QObject {
    Q_OBJECT

public:
    GameClient(QObject *parent = nullptr);

    void connectToServer(const QString& address, quint16 port);
    void listenForBroadcasts();
    void resetServer();
    bool badDisconected();
    bool serverStart();

public slots:
    void sendChoice(const QString&);
    void resetConnectionFlag();

private slots:
    void handleServerMessage();
    void handleBroadcastDatagram();

signals:
    void showChoicesWindow();
    void showResultWindow(const QString&);
    void showMainWindow();
    void serverFound(const QString& address, quint16 port);
    void showMessage(const QString&);
    void disconnected();

private:
    QTcpSocket *socket;
    QUdpSocket *udpSocket;
    bool startServer;
    bool BadDisconected;
};

#endif
