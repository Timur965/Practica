#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
    setWindowTitle("Выбор аппаратуры и режима");                                //Установили заголовок окна

    equipment = new QStringList();
    if(file->outputFile("equipmentAndMode.txt",equipment))                      //Считали данные из файла
    {
        foreach(QString str,*equipment)
            ui->equipment->addItem(str);                                        //Добавили данные в combobox
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
        delete w;                                                               //Освобождение памяти из под w
    if(file != nullptr)
        delete file;                                                            //Освобождение памяти из под file
    if(mode != nullptr)
        delete mode;                                                            //Освобождение памяти из под mode
    if(equipment != nullptr)
        delete equipment;                                                       //Освобождение памяти из под equipment
}

void SecondWindow::on_pushButton_clicked()                                      //Слот для открытия нового окна
{
    w = new MainWindow();
    w->show();
    w->viewEquipment(ui->equipment->currentIndex());
    w->mode(ui->mode->currentText());
    this->close();
}

void SecondWindow::on_equipment_currentIndexChanged(const QString &arg1)        //Слот для заполнения combobox с режимами
{
    ui->mode->clear();
    mode = new QStringList();
    if(file->outputCurrentEquipment("equipmentAndMode.txt",mode,arg1))          //Считали данные из файла
    {
        foreach(QString str,*mode)
            ui->mode->addItem(str);                                             //Добавили данные в combobox
    }
    else
    {
        QMessageBox::warning(this,"Ошибка с файлом","Файла не существует");
    }
}
