#include "test_database.h"

Test_Database::Test_Database()
{

}
void Test_Database::connection()                                                        //Слот для проверки соединения с БД
{
    QCOMPARE(database.connection("postgres","timurka01","127.0.0.1","Example"),true);   //Сравниваем актуальное значение с фактическим
}
void Test_Database::insertTable()                                                       //Слот для проверки вставки данных в БД
{
    QStringList columns;
    columns.push_back("id");
    columns.push_back("firstname");
    QStringList value;
    value.push_back("1");
    value.push_back("Ivan");
    QCOMPARE(database.insertTable("ExampleTable",columns,value),true);                  //Сравниваем актуальное значение с фактическим
    insertValue.push_back(QString("%1,%2").arg(value.at(0),value.at(1)));
}
void Test_Database::outputFromTable()                                                   //Слот для проверки считывания данных из БД
{
    QStringList columns;
    QStringList tables;
    tables.push_back("ExampleTable");
    columns.push_back("id");
    columns.push_back("firstname");
    database.outputFromTable(tables,columns,"",&outputValue);
    QCOMPARE(insertValue,outputValue);                                                  //Сравниваем актуальное значение с фактическим
}
void Test_Database::updateTable()                                                       //Слот для проверки обновления таблиц в БД
{
    QCOMPARE(database.updateTable("ExampleTable","name","Ivan","Petr"),true);           //Сравниваем актуальное значение с фактическим
}
void Test_Database::deleteRow()                                                         //Слот для проверки удаления строки из БД
{
    QCOMPARE(database.deleteRow("ExampleTable","id","1","="),true);                     //Сравниваем актуальное значение с фактическим
    database.closeConnection();                                                         //Разрываем соединение с БД
}
