#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include "mainwindow.h"
#include "fileinout.h"
#include <QString>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();

private slots:
    void on_pushButton_clicked();                                   //Слот для открытия нового окна
    void timerTimeout();
private:
    Ui::SecondWindow *ui;
    MainWindow *w = nullptr;
    QTimer *timer = nullptr;
};

#endif // SECONDWINDOW_H
