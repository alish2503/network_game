#ifndef WAITWINDOW_H
#define WAITWINDOW_H

#include <QDialog>

namespace Ui {
class WaitWindow;
}

class WaitWindow : public QDialog
{
    Q_OBJECT

signals:
    void sendDisconect(const QString&);

public:
    explicit WaitWindow(QWidget *parent = nullptr);
    ~WaitWindow();
    void enableButton();

private slots:
    void on_Quit_clicked();

private:
    Ui::WaitWindow *ui;
};

#endif // WAITWINDOW_H
