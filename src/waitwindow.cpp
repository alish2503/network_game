#include "waitwindow.h"
#include "ui_waitwindow.h"

WaitWindow::WaitWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitWindow)
{
    ui->setupUi(this);
}

WaitWindow::~WaitWindow()
{
    delete ui;
}

void WaitWindow::on_Quit_clicked()
{
    emit sendDisconect("disconect");
    ui->Quit->setDefault(true);
}
void WaitWindow::enableButton() {
    ui->Quit->setEnabled(true);
}

