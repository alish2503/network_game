#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();
    void showMessage1(const QString&);
    void showMessage2(const QString&);

private slots:
    void on_Restart_clicked();

    void on_Quit_clicked();

signals:
    void finishGame(const QString&);

private:
    Ui::ResultWindow *ui;

};

#endif // RESULTWINDOW_H
