#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      server(new GameServer(this)),
      client(new GameClient(this)),
      waitWindow(nullptr),
      choicesWindow(nullptr),
      resultWindow(nullptr)
{

    ui->setupUi(this);

    //connect to the server when the client emits a signal if it has found this server
    connect(client, &GameClient::serverFound, client, &GameClient::connectToServer);

    connect(client, &GameClient::showChoicesWindow, this, &MainWindow::showChoicesWindow);
    connect(client, &GameClient::showResultWindow, this, &MainWindow::showResultWindow);
    connect(client, &GameClient::showMainWindow, this, &MainWindow::showMainWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Play_clicked()
{
    client->listenForBroadcasts();

    //a small delay has been set so that the client has time to emit a signal if it has found a server so as not to launch another server
    QTimer::singleShot(BROADCAST_LISTEN_TIMEOUT_MS, this, [this] {
        if (client->serverStart()) {
            server->start();
        }
      });
    waitWindow = new WaitWindow(this);

    //the user can click on the quit button in the interface only if he has connected to the server that he also launched.
    //this is only necessary if this player has launched the game and no one connects to him
    connect(server, &GameServer::clientConnected, waitWindow, &WaitWindow::enableButton, Qt::UniqueConnection);

    connect(waitWindow, &WaitWindow::sendDisconect, client, &GameClient::sendChoice, Qt::UniqueConnection);
    close();
    waitWindow->show();
    ui->Message->clear();
}


void MainWindow::on_Quit_clicked()
{
    server->closeServer();
    QCoreApplication::quit();
}

void MainWindow::showChoicesWindow() {
    choicesWindow = new ChoicesWindow(this);
    connect(choicesWindow, &ChoicesWindow::sendChoice, client, &GameClient::sendChoice, Qt::UniqueConnection);
    if(waitWindow) {
        waitWindow->close();
        delete waitWindow;
        waitWindow = nullptr;
    }
    else if(resultWindow) {
        resultWindow->close();
        choicesWindow->resetAllButtons();
    }
    choicesWindow->show();
}

void MainWindow::showResultWindow(const QString &message) {
    resultWindow = new ResultWindow(this);
    choicesWindow->close();
    choicesWindow->resetAllButtons();
    resultWindow->show();
    resultWindow->showMessage1(message);
    connect(resultWindow, &ResultWindow::finishGame, client, &GameClient::sendChoice, Qt::UniqueConnection);
    connect(client, &GameClient::showMessage, resultWindow, &ResultWindow::showMessage2, Qt::UniqueConnection);
}
void MainWindow::showMainWindow() {

    if(resultWindow) {
        resultWindow->close();
        delete  resultWindow;
        resultWindow = nullptr;
    }
    if(choicesWindow) {
        choicesWindow->close();
        delete choicesWindow;
        choicesWindow = nullptr;
    }
    if(waitWindow) {
        waitWindow->close();
        delete waitWindow;
        waitWindow = nullptr;
    }
    if(client->badDisconected()) {
        ui->Message->setText("Connection lost. Try again.");
    }
    client->resetServer();
    connect(client, &GameClient::disconnected, client, &GameClient::resetConnectionFlag, Qt::UniqueConnection);
    show();
}

