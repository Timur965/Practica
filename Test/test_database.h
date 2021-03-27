#ifndef TEST_DATABASE_H
#define TEST_DATABASE_H

#include <QtTest>
#include "database.h"

class Test_Database: public QObject
{
    Q_OBJECT
public:
    Test_Database();

private slots:
    void connection();          //Слот для проверки соединения с БД
    void createTable();         //Слот для проверки создания таблицы с БД
    void insertTable();         //Слот для проверки вставки данных в БД
    void outputFromTable();     //Слот для проверки считывания данных из БД
    void updateTable();         //Слот для проверки обновления таблиц в БД
    void deleteRow();           //Слот для проверки удаления строки из БД
private:
    DataBase database;
    QStringList insertValue;    //Записываемые данные в БД
    QStringList outputValue;    //Считываемые данные из БД
};

#endif // TEST_DATABASE_H
