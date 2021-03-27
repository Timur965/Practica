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
    void on_equipment_currentIndexChanged(const QString &arg1);     //Слот для заполнения combobox с режимами

private:
    Ui::SecondWindow *ui;
    MainWindow *w = nullptr;
    FileInOut *file=nullptr;
    QStringList *mode = nullptr;
    QStringList *equipment = nullptr;
};

#endif // SECONDWINDOW_H
