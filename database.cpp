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
bool DataBase::createTable(QString nameTable, QStringList columns)                                                                                                      //Метод для создания таблицы в БД
{
    if(isOpen)
    {
        QSqlQuery query;                                                                                                                                                //Создаём запрос
        QString textQuery;
        textQuery = QString("CREATE TABLE \"%1\" (").arg(nameTable);
        foreach(QString str,columns)
        {
            if(str!=columns.last())
                textQuery += QString("%1,").arg(str);
            else
                textQuery += QString("%1").arg(str);
        }
        textQuery +=")";
        if(query.exec(textQuery))                                                                                                                                       //Выполняем запрос
        {
            return true;
        }
    }
    return false;
}
bool DataBase::insertTable(QString nameTable, QStringList value)                                                                                                       //Метод для добавления данных в БД
{
    if(isOpen)
    {
        QSqlQuery query;                                                                                                                                               //Создаём запрос
        QString textQuery;
        int i = 0;
        foreach(QString str, value)
        {
            if(i != value.count() - 1)
                textQuery += QString("'%1',").arg(str);
            else
                textQuery += QString("'%1'").arg(str);
            i++;
        }
        if(query.exec(QString("INSERT INTO \"%1\" VALUES(%2)").arg(nameTable,textQuery)))                                                                              //Выполняем запрос
        {
            return true;
        }
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
        qDebug()<<query.lastError().text();
    }
    return false;
}
bool DataBase::outputFromTable(QString nameDatabase, QString nameTable, QStringList *result)
{
    if(isOpen)
    {
        QSqlQuery query;                                                                                                                                                //Создаём запрос
        query.exec(QString("SELECT COUNT(data_type) FROM information_schema.columns WHERE table_catalog = '%1' AND table_name = '%2'").arg(nameDatabase,nameTable));    //Выполняем запрос
        query.next();
        int n = query.value(0).toInt();                                                                                                                                 //Получаем значение
        QString column;
        if(query.exec(QString("SELECT * FROM \"%1\" ").arg(nameTable)))                                                                                                 //Выполняем запрос
        {
            while (query.next()) {
                for(int i = 0; i < n; i++)
                {
                    if(i != n-1)
                        column += query.value(i).toString()+",";                                                                                                        //Получаем значение
                    else
                        column += query.value(i).toString();                                                                                                            //Получаем значение
                }
                result->push_back(column);
                column="";
            }
            return true;
        }
    }
    return false;
}
