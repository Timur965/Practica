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
    bool deleteRow(QString nameTable, QString id);
    QString Error();
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
