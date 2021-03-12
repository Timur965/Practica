#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
    setWindowTitle("Выбор аппаратуры и режима");

    equipment = new QStringList();
    if(file->outputFile("equipmentAndMode.txt",equipment))
    {
        foreach(QString str,*equipment)
            ui->equipment->addItem(str);
    }
    else
    {
        QMessageBox::warning(this,"Ошибка с файлом","Файла не существует");
    }
}

SecondWindow::~SecondWindow()
{
    delete ui;
    if(w != nullptr)
        delete w;
    if(file != nullptr)
        delete file;
    if(mode != nullptr)
        delete mode;
    if(equipment != nullptr)
        delete equipment;
}

void SecondWindow::on_pushButton_clicked()
{
    w = new MainWindow();
    w->show();
    w->mode(ui->mode->currentText());
    this->close();
}

void SecondWindow::on_equipment_currentIndexChanged(const QString &arg1)
{
    ui->mode->clear();
    mode = new QStringList();
    if(file->outputCurrentEquipment("equipmentAndMode.txt",mode,arg1))
    {
        foreach(QString str,*mode)
            ui->mode->addItem(str);
    }
    else
    {
        QMessageBox::warning(this,"Ошибка с файлом","Файла не существует");
    }
}
