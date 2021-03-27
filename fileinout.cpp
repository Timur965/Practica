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
            jsObj.insert("x",ops->pos().x());                                                   //Вставляем элемент с ключом x
            jsObj.insert("y",ops->pos().y());                                                   //Вставляем элемент с ключом y
            jsObj.insert("width",ops->width/Operation::getCoef());                              //Вставляем элемент с ключом width
            jsObj.insert("height",ops->height);                                                 //Вставляем элемент с ключом height
            jsObj.insert("dynamic",ops->dynamic);                                               //Вставляем элемент с ключом dynamic
            jsObj.insert("inQueue",ops->inQueue);                                               //Вставляем элемент с ключом inQueue

            jsArray.push_back(jsObj);
        }
        jsObj1.insert("Operations",jsArray);
        doc = QJsonDocument(jsObj1);                                                            //Создаем QJsonDocument из объекта
        jsStr = doc.toJson(QJsonDocument::Compact);                                             //Преобразуем QJsonDocument в документ JSON в кодировке UTF-8 и определяем формат
        stream<<jsStr;                                                                          //Записываем в stream
        file1.close();
        return true;
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
            geom.x = js["x"].toDouble();
            geom.y = js["y"].toDouble();
            geom.width = js["width"].toDouble();
            geom.height = js["height"].toDouble();
            geom.dynamic = js["dynamic"].toBool();
            geom.inQueue = js["inQueue"].toBool();
            vectorGeom->push_back(geom);
        }
        file1.close();
        return true;
    }
    return false;
}
bool FileInOut::outputFile(QString path, QStringList *list)                                    //Считывание из обычного файла
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QString textLine;
        while (!file.atEnd())                                                                  //Пока не конец файла
        {
            textLine = file.readLine();                                                        //Читаем построчно
            list->push_back(textLine.left(textLine.indexOf(':')));
        }
        file.close();
        return true;
    }
    return false;
}
bool FileInOut::outputCurrentEquipment(QString path, QStringList *list, const QString &arg1)   //Считывание из файла текушей аппаратуры
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QString textLine;
        while(!file.atEnd())                                                                   //Пока не конец файла
        {
            textLine = file.readLine();                                                        //Читаем построчно
            if(textLine.indexOf(arg1) != -1)
            {
                textLine = textLine.remove(0,textLine.indexOf(':')+2);                         //Удаляем из строки
                list->append(textLine.split(','));                                             //Разделяем строку
                break;
            }
        }
        file.close();
        return true;
    }
    return false;
}
