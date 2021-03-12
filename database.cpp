#include "database.h"

DataBase::DataBase()
{

}
DataBase::~DataBase()
{
    db.close();
}
bool DataBase::connection(QString login, QString password, QString host, QString databaseName)
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setDatabaseName(databaseName);
    db.setUserName(login);
    db.setPassword(password);
    db.setPort(5432);
    if(db.open())
    {
        return true;
    }
    return false;
}
bool DataBase::createTable(QString nameTable, QStringList columns)
{
    QSqlQuery query;
    if(db.isOpen())
    {
        QString textQuery;
        textQuery = "CREATE TABLE "+nameTable+" (";
        foreach(QString str,columns)
        {
            if(str!=columns.last())
                textQuery += str+",";
            else
                textQuery +=str;
        }
        textQuery +=")";
        if(query.exec(textQuery))
        {
            return true;
        }
    }
    return false;
}
bool DataBase::insertTable(QString nameTable, QStringList value)
{
    QSqlQuery query;
    if(db.isOpen())
    {
        QString textQuery;
        foreach(QString str, value)
        {
            if(str!=value.last())
                textQuery += "\'"+str+"\',";
            else
                textQuery += "\'"+str+"\'";
        }
        if(query.exec("INSERT INTO "+nameTable+" VALUES("+textQuery+")"))
        {
            return true;
        }
    }
    return false;
}
bool DataBase::updateTable(QString nameTable, QString nameColumn, QString value, QString newValue)
{
    QSqlQuery query;
    if(db.isOpen())
    {
        if(query.exec("UPDATE "+nameTable+" SET "+nameColumn+"="+"\'"+newValue+"\'"+" WHERE "+nameColumn+" = "+"\'"+value+"\'"))
        {
            return true;
        }
    }
    return false;
}
bool DataBase::deleteRow(QString nameTable, QString index)
{
    QSqlQuery query;
    if(db.isOpen())
    {
        if(query.exec("DELETE FROM "+nameTable+" WHERE id="+index))
        {
            return true;
        }
    }
    return false;
}
