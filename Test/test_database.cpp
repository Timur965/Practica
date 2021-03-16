#include "test_database.h"

Test_Database::Test_Database()
{

}
void Test_Database::connection()
{
    QCOMPARE(database.connection("postgres","timurka01","127.0.0.1","Example"),true);
}
void Test_Database::createTable()
{
    QStringList columns;
    columns.push_back("Id SERIAL PRIMARY KEY");
    columns.push_back("FirstName CHARACTER VARYING(30)");
    QCOMPARE(database.createTable("ExampleTable",columns),true);
}
void Test_Database::insertTable()
{
    QStringList columns;
    columns.push_back("1");
    columns.push_back("Ivan");
    QCOMPARE(database.insertTable("ExampleTable",columns),true);
    insertValue.push_back(columns.at(0)+","+columns.at(1));
}
void Test_Database::outputFromTable()
{
    database.outputFromTable("Example","ExampleTable",&outputValue);
    QCOMPARE(insertValue,outputValue);
}
void Test_Database::updateTable()
{
    QCOMPARE(database.updateTable("ExampleTable","FirstName","Ivan","Petr"),true);
}
void Test_Database::deleteRow()
{
    QCOMPARE(database.deleteRow("ExampleTable","1"),true);
}
