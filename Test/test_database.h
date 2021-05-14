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
    void connection();          //Метод для проверки соединения с БД
    void insertTable();         //Метод для проверки вставки данных в БД
    void updateTable();         //Метод для проверки обновления таблиц в БД
    void deleteRow();           //Метод для проверки удаления строки из БД
private:
    DataBase database;
};

#endif // TEST_DATABASE_H
