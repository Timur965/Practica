#include "fileinout.h"

FileInOut::FileInOut()
{

}
FileInOut::~FileInOut()
{
}
bool FileInOut::inputJSONFile(QVector<Operation *> operations, QString path)                    //Запись в JSON файл
{
    QFile file1(path);                                                                          //Объект с путём к файлу
    QTextStream stream(&file1);                                                                 //Создаём QTextStream, который взаимодействует с устройством file1
    QJsonObject jsObj;                                                                          //Создаём JSON объект
    QJsonObject jsObj1;                                                                         //Создаём JSON объект
    QJsonArray jsArray;                                                                         //Создаём JSON массив
    QJsonDocument doc;                                                                          //Создаём JSON документ
    QString jsStr;
    if(file1.open(QIODevice::WriteOnly))
    {
        foreach(Operation *ops, operations)
        {
            jsObj.insert("name",ops->name);                                                     //Вставляем элемент с ключом name
            jsObj.insert("reduction",ops->reduction);                                           //Вставляем элемент с ключом width
            jsObj.insert("width",ops->width/Operation::getCoef());                              //Вставляем элемент с ключом width
            jsObj.insert("interval",ops->interval/Operation::getCoef());                        //Вставляем элемент с ключом height
            jsObj.insert("dynamic",ops->dynamic);                                               //Вставляем элемент с ключом dynamic

            jsArray.push_back(jsObj);
        }
        jsObj1.insert("Operations",jsArray);
        doc = QJsonDocument(jsObj1);                                                            //Создаем QJsonDocument из объекта
        jsStr = doc.toJson(QJsonDocument::Compact);                                             //Преобразуем QJsonDocument в документ JSON в кодировке UTF-8 и определяем формат
        stream.setCodec("UTF-8");
        stream<<jsStr;                                                                          //Записываем в stream
        file1.close();
        return true;
    }
    return false;
}
bool FileInOut::inputFile(QVector<Operation *> operations, QString path)
{
    QFile file1(path);                                                                         //Объект с путём к файлу
    if(file1.open(QIODevice::WriteOnly))
    {
        if(!operations.isEmpty())
        {
            QTextStream stream(&file1);
            QString transmissionСhannel;
            foreach(Operation *ops, operations)
            {
                if(ops->reduction.contains("Т0") || ops->reduction.contains("РК") || ops->reduction.contains("КодМВ"))
                    transmissionСhannel = "RS-232";
                if(ops->reduction.contains("КПИ"))
                    transmissionСhannel = "МКО БВС";
                if(ops->reduction.contains("ЗИ"))
                    transmissionСhannel = "МКО СТИ";
                if(ops->reduction.contains("ВИ"))
                    transmissionСhannel = "USB";
                if(ops->dynamic)
                    stream<<ops->start<<"\t-\t"<<ops->end<<"\t-\t"<<ops->reduction<<"\t-\t"<<transmissionСhannel<<"\n";
                else
                    stream<<ops->start<<"\t-\t"<<ops->start<<"\t-\t"<<ops->reduction<<"\t-\t"<<transmissionСhannel<<"\n";
            }
            file1.close();
            return true;
        }
    }
    return false;
}
bool FileInOut::outputJSONFile(QString path, QVector<Geometry> *vectorGeom)                    //Считывание из JSON файла
{
    QFile file1(path);                                                                         //Объект с путём к файлу
    QJsonObject jsObj;                                                                         //Создаём JSON объект
    QJsonDocument doc;                                                                         //Создаём JSON документ
    QString strJs;
    QJsonArray jsArray;                                                                        //Создаём JSON массив
    Geometry geom;
    if(file1.open(QIODevice::ReadOnly))
    {
        strJs = file1.readAll();                                                               //Считываем весь файл
        doc= QJsonDocument::fromJson(strJs.toUtf8());                                          //Анализируем JSON-документ и создаём из него QJsonDocument
        jsObj = doc.object();                                                                  //Получаем QJsonObject, содержащийся в документе
        jsArray = jsObj["Operations"].toArray();                                               //Разбираем объект на массив
        foreach(QJsonValue js, jsArray)
        {
            geom.name = js["name"].toString();
            geom.reduction = js["reduction"].toString();
            geom.width = js["width"].toDouble();
            geom.interval = js["interval"].toDouble();
            geom.dynamic = js["dynamic"].toBool();
            vectorGeom->push_back(geom);
        }
        file1.close();
        return true;
    }
    return false;
}
bool FileInOut::outputNamesOperation(QString path, QVector<Geometry> *vectorGeom)
{
    QFile file1(path);                                                                         //Объект с путём к файлу
    QJsonObject jsObj;                                                                         //Создаём JSON объект
    QJsonDocument doc;                                                                         //Создаём JSON документ
    QString strJs;
    QJsonArray jsArray;                                                                        //Создаём JSON массив
    Geometry geom;
    if(file1.open(QIODevice::ReadOnly))
    {
        strJs = file1.readAll();                                                               //Считываем весь файл
        doc= QJsonDocument::fromJson(strJs.toUtf8());                                          //Анализируем JSON-документ и создаём из него QJsonDocument
        jsObj = doc.object();                                                                  //Получаем QJsonObject, содержащийся в документе
        jsArray = jsObj["namesOperations"].toArray();                                          //Разбираем объект на массив
        foreach(QJsonValue js, jsArray)
        {
            geom.name = js["name"].toString();
            geom.reduction = js["reduction"].toString();
            geom.dynamic = js["dynamic"].toBool();
            vectorGeom->push_back(geom);
        }
        file1.close();
        return true;
    }
    return false;
}
