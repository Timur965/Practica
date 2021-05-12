#include "database.h"

DataBase::DataBase()
{

}
DataBase::~DataBase()
{
}
bool DataBase::connection(QString login, QString password, QString host, QString databaseName)                                                                          //Метод для установки соединения с БД
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");                                                                                                               //Добавляем базу данных в список соединений баз данных
    db.setHostName(host);                                                                                                                                               //Устанавливаем хост
    db.setDatabaseName(databaseName);                                                                                                                                   //Устанавливаем название БД
    db.setUserName(login);                                                                                                                                              //Устанавливаем логин
    db.setPassword(password);                                                                                                                                           //Устанавливаем пароль
    db.setPort(5432);                                                                                                                                                   //Устанавливаем порт
    if(db.open())                                                                                                                                                       //Открываем соединение с БД
    {
        isOpen = true;
        return true;
    }
    return false;
}
void DataBase::closeConnection()                                                                                                                                        //Метод для разрыва соединения с БД
{
    isOpen = false;
    QSqlDatabase::removeDatabase("qt_sql_default_connection");                                                                                                          //Убираем соединение
}
bool DataBase::insertTable(QString nameTable, QStringList columns, QStringList value)                                                                                   //Метод для добавления данных в БД
{
    if(isOpen)
    {
        QSqlQuery query;                                                                                                                                                //Создаём запрос
        QString textQuery;
        QString textColumns;
        int i = 0;
        if(!columns.empty())
        {
            foreach(QString str, columns)
            {
                if(i != columns.count() - 1)
                    textColumns += QString("\"%1\",").arg(str);
                else
                    textColumns += QString("\"%1\"").arg(str);
                i++;
            }
        i=0;
        }
        foreach(QString str, value)
        {
            if(i != value.count() - 1)
                textQuery += QString("'%1',").arg(str);
            else
                textQuery += QString("'%1'").arg(str);
            i++;
        }
        if(query.exec(QString("INSERT INTO \"%1\"(%2) VALUES(%3)").arg(nameTable,textColumns,textQuery)))                                                               //Выполняем запрос
        {
            return true;
        }
        qDebug()<<query.lastError().text();
    }
    return false;
}
bool DataBase::updateTable(QString nameTable, QString nameColumn, QString value, QString newValue)
{
    if(isOpen)
    {
        QSqlQuery query;                                                                                                                                                //Создаём запрос
        if(query.exec(QString("UPDATE \"%1\" SET %2 = '%3' WHERE %4 = '%5'").arg(nameTable,nameColumn,newValue,nameColumn,value)))                                      //Выполняем запрос
        {
            return true;
        }
    }
    return false;
}
bool DataBase::deleteRow(QString nameTable,QString nameColumn, QString index, QString sign)
{
    if(isOpen)
    {
        QSqlQuery query;                                                                                                                                                //Создаём запрос
        if(query.exec(QString("DELETE FROM \"%1\" WHERE \"%2\" %3 %4").arg(nameTable,nameColumn,sign,index)))                                                           //Выполняем запрос
        {
            return true;
        }
    }
    return false;
}
bool DataBase::outputFromTable(QStringList nameTable, QStringList columns, QString condition, QStringList *result)
{
    if(isOpen)
    {
        QSqlQuery query;                                                                                                                                                //Создаём запрос                                                                                                                                //Получаем значение
        QString column;
        QString tables;
        QString value;
        int i=0;
        foreach(QString str, columns)
        {
            if(i != columns.count() - 1)
                column += QString("\"%1\",").arg(str);
            else
                column += QString("\"%1\"").arg(str);
            i++;
        }
        i=0;
        foreach(QString str, nameTable)
        {
            if(i != nameTable.count() - 1)
                tables += QString("\"%1\",").arg(str);
            else
                tables += QString("\"%1\"").arg(str);
            i++;
        }
        if(query.exec(QString("SELECT %1 FROM %2 %3").arg(column, tables, condition)))                                                                                                 //Выполняем запрос
        {
            while (query.next()) {
                for(int i = 0; i < columns.count(); i++)
                {
                    if(i != columns.count()-1)
                        value += query.value(i).toString()+",";                                                                                                        //Получаем значение
                    else
                        value += query.value(i).toString();                                                                                                            //Получаем значение
                }
                result->push_back(value);
                value="";
            }
            return true;
        }
        qDebug()<<query.lastError().text();
    }
    return false;
}
