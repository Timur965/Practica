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
    bool createTable(QString nameTable, QStringList columns);                                   //Метод для создание таблиц в БД
    bool insertTable(QString nameTable, QStringList value);                                     //Метод для вставки данных в таблицу
    bool updateTable(QString nameTable, QString nameColumn, QString Value, QString newValue);   //Метод для обновления данных в таблице
    bool deleteRow(QString nameTable, QString nameColumn, QString id, QString sign);            //Метод для удаления строки
    bool outputFromTable(QString nameDatabase, QString nameTable, QStringList *result);         //Метод для считывания данныз из БД
    void closeConnection();                                                                     //Метод для разрыва соединения с БД
    bool isOpen=false;                                                                          //Переменная для проверки установлено ли оединение с БД
};

#endif // DATABASE_H
