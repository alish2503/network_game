#include "choiceswindow.h"
#include "ui_choiceswindow.h"

ChoicesWindow::ChoicesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoicesWindow)
{
    ui->setupUi(this);
}

ChoicesWindow::~ChoicesWindow()
{
    delete ui;
}

void ChoicesWindow::on_Rock_clicked()
{
    emit sendChoice("Rock");
    ui->Rock->setDefault(true);
    ui->Scissors->setEnabled(false);
    ui->Paper->setEnabled(false);
}

void ChoicesWindow::on_Scissors_clicked()
{
    emit sendChoice("Scissors");
    ui->Scissors->setDefault(true);
    ui->Rock->setEnabled(false);
    ui->Paper->setEnabled(false);
}


void ChoicesWindow::on_Paper_clicked()
{
    emit sendChoice("Paper");
    ui->Paper->setDefault(true);
    ui->Rock->setEnabled(false);
    ui->Scissors->setEnabled(false);
}

void ChoicesWindow::resetAllButtons() {
    ui->Rock->setEnabled(true);
    ui->Scissors->setEnabled(true);
    ui->Paper->setEnabled(true);
    ui->Rock->setDefault(false);
    ui->Scissors->setDefault(false);
    ui->Paper->setDefault(false);
}

