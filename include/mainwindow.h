#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameserver.h"
#include "gameclient.h"
#include "waitwindow.h"
#include "choiceswindow.h"
#include "resultwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Play_clicked();

    void on_Quit_clicked();
    void showChoicesWindow();
    void showMainWindow();
    void showResultWindow(const QString&);

private:
    Ui::MainWindow *ui;
    GameServer *server;
    GameClient *client;
    WaitWindow *waitWindow;
    ChoicesWindow *choicesWindow;
    ResultWindow *resultWindow;
};
#endif // MAINWINDOW_H
