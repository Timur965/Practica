#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

class DataBase
{
public:
    DataBase();
    ~DataBase();
    bool connection(QString login, QString password, QString host, QString databaseName);
    bool createTable(QString nameTable, QStringList columns);
    bool insertTable(QString nameTable, QStringList value);
    bool updateTable(QString nameTable, QString nameColumn, QString Value, QString newValue);
    bool deleteRow(QString nameTable, QString id);
    bool outputFromTable(QString nameDatabase, QString nameTable, QStringList *result);
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
