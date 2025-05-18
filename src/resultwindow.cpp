#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::on_Restart_clicked()
{
    emit finishGame("restart");
    ui->Restart->setDefault(true);
}

void ResultWindow::on_Quit_clicked()
{
    emit finishGame("quit");
}

void ResultWindow::showMessage1(const QString &message) {
    ui->Message1->setText(message);
}

void ResultWindow::showMessage2(const QString &message) {
   ui->Message2->setText(message);
}

