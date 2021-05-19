#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class DataBase
{
public:
    DataBase();
    ~DataBase();
    bool connection(QString login, QString password, QString host, QString databaseName);       //Метод для установки соединения с БД
    bool insertTable(QString nameTable,QStringList columns, QStringList value);                 //Метод для вставки данных в таблицу
    void closeConnection();                                                                     //Метод для разрыва соединения с БД
    bool isOpen=false;                                                                          //Переменная для проверки установлено ли оединение с БД
};

#endif // DATABASE_H
