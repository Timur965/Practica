#include "test_database.h"

Test_Database::Test_Database()
{

}
void Test_Database::connection()
{
    DataBase database;
    QCOMPARE(database.connection("postgres","220201","127.0.0.1","Example"),true);

}
void Test_Database::createTable()
{
    DataBase database;
    QStringList columns;
    columns.push_back("Id SERIAL PRIMARY KEY");
    columns.push_back("FirstName CHARACTER VARYING(30)");
    QCOMPARE(database.createTable("ExampleTable",columns),true);
}
void Test_Database::insertTable()
{
    DataBase database;
    QStringList columns;
    columns.push_back("1");
    columns.push_back("Ivan");
    QCOMPARE(database.insertTable("ExampleTable",columns),true);
}
void Test_Database::updateTable()
{
    DataBase database;
    QCOMPARE(database.updateTable("ExampleTable","FirstName","Ivan","Petr"),true);
}
void Test_Database::deleteRow()
{
    DataBase database;
    QCOMPARE(database.deleteRow("ExampleTable","1"),true);
}
