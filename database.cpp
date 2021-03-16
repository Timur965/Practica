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
    if(db.isOpen())
    {
        QSqlQuery query;
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
    if(db.isOpen())
    {
        QSqlQuery query;
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
    if(db.isOpen())
    {
        QSqlQuery query;
        if(query.exec("UPDATE "+nameTable+" SET "+nameColumn+"="+"\'"+newValue+"\'"+" WHERE "+nameColumn+" = "+"\'"+value+"\'"))
        {
            return true;
        }
    }
    return false;
}
bool DataBase::deleteRow(QString nameTable, QString index)
{
    if(db.isOpen())
    {
        QSqlQuery query;
        if(query.exec("DELETE FROM "+nameTable+" WHERE id="+index))
        {
            return true;
        }
    }
    return false;
}
bool DataBase::outputFromTable(QString nameDatabase, QString nameTable, QStringList *result)
{
    if(db.isOpen())
    {
        QSqlQuery query;
        query.exec("SELECT COUNT(data_type) FROM information_schema.columns WHERE table_catalog = '"+nameDatabase+"' AND table_name = '"+nameTable.toLower()+"'");
        query.next();
        int n = query.value(0).toInt();
        QString column;
        if(query.exec("SELECT * FROM "+nameTable))
        {
            while (query.next()) {
                for(int i = 0; i < n; i++)
                {
                    if(i != n-1)
                        column += query.value(i).toString()+",";
                    else
                        column += query.value(i).toString();
                }
                result->push_back(column);
                qDebug()<<column;
                column="";
            }
            return true;
        }
    }
    return false;
}
