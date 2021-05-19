#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);                 //Инициализируем оконную систему и создаём объект приложения с argc параметрами командной строки в argv
    MainWindow w;
    w.show();                                   //Отображаем MainWindow
    return a.exec();
}
