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
    bool connection(QString login, QString password, QString host, QString databaseName);
    bool createTable(QString nameTable, QStringList columns);
    bool insertTable(QString nameTable, QStringList value);
    bool updateTable(QString nameTable, QString nameColumn, QString Value, QString newValue);
    bool deleteRow(QString nameTable, QString nameColumn, QString id, QString sign);
    bool outputFromTable(QString nameDatabase, QString nameTable, QStringList *result);
    void closeConnection();
    bool isOpen=false;
};

#endif // DATABASE_H
