#ifndef CHOICESWINDOW_H
#define CHOICESWINDOW_H

#include <QDialog>

namespace Ui {
class ChoicesWindow;
}

class ChoicesWindow : public QDialog
{
    Q_OBJECT
signals:
    void sendChoice(const QString&);

public:
    explicit ChoicesWindow(QWidget *parent = nullptr);
    ~ChoicesWindow();
    void resetAllButtons();

private slots:
    void on_Rock_clicked();

    void on_Scissors_clicked();

    void on_Paper_clicked();

private:
    Ui::ChoicesWindow *ui;
};

#endif // CHOICESWINDOW_H
