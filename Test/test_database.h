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
    void connection();
    void createTable();
    void insertTable();
    void outputFromTable();
    void updateTable();
    void deleteRow();
private:
    DataBase database;
    QStringList insertValue;
    QStringList outputValue;
};

#endif // TEST_DATABASE_H
