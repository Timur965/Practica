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
