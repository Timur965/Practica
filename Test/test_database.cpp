#include "test_database.h"

Test_Database::Test_Database()
{

}
void Test_Database::connection()                                                        //Слот для проверки соединения с БД
{
    QCOMPARE(database.connection("postgres","timurka01","127.0.0.1","Example"),true);   //Сравниваем актуальное значение с фактическим
}
void Test_Database::createTable()                                                       //Слот для проверки создания таблицы с БД
{
    QStringList columns;
    columns.push_back("Id INTEGER PRIMARY KEY");
    columns.push_back("FirstName CHARACTER VARYING(30)");
    QCOMPARE(database.createTable("ExampleTable",columns),true);                        //Сравниваем актуальное значение с фактическим
}
void Test_Database::insertTable()                                                       //Слот для проверки вставки данных в БД
{
    QStringList columns;
    columns.push_back("1");
    columns.push_back("Ivan");
    QCOMPARE(database.insertTable("ExampleTable",columns),true);                        //Сравниваем актуальное значение с фактическим
    insertValue.push_back(QString("%1,%2").arg(columns.at(0),columns.at(1)));
}
void Test_Database::outputFromTable()                                                   //Слот для проверки считывания данных из БД
{
    database.outputFromTable("Example","ExampleTable",&outputValue);
    QCOMPARE(insertValue,outputValue);                                                  //Сравниваем актуальное значение с фактическим
}
void Test_Database::updateTable()                                                       //Слот для проверки обновления таблиц в БД
{
    QCOMPARE(database.updateTable("ExampleTable","FirstName","Ivan","Petr"),true);      //Сравниваем актуальное значение с фактическим
}
void Test_Database::deleteRow()                                                         //Слот для проверки удаления строки из БД
{
    QCOMPARE(database.deleteRow("ExampleTable","id","1","="),true);                     //Сравниваем актуальное значение с фактическим
    database.closeConnection();                                                         //Разрываем соединение с БД
}
