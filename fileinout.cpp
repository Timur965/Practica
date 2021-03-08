#include "fileinout.h"

FileInOut::FileInOut()
{

}
FileInOut::~FileInOut()
{
}
bool FileInOut::inputFile(QVector<Operation *> operations, QString path)
{
    QFile file1(path);
    QTextStream stream(&file1);
    QJsonObject jsObj;
    QJsonObject jsObj1;
    QJsonArray jsArray;
    QJsonDocument doc;
    QString jsStr;
    if(file1.open(QIODevice::WriteOnly))
    {
        foreach(Operation *ops, operations)
        {
            jsObj.insert("name",ops->name);
            jsObj.insert("x",ops->pos().x());
            jsObj.insert("y",ops->pos().y());
            jsObj.insert("width",ops->width);
            jsObj.insert("height",ops->height);
            jsObj.insert("dynamic",ops->dynamic);
            jsObj.insert("inQueue",ops->inQueue);

            jsArray.push_back(jsObj);
        }
        jsObj1.insert("Operations",jsArray);
        doc = QJsonDocument(jsObj1);
        jsStr = doc.toJson(QJsonDocument::Compact);
        stream<<jsStr;
        file1.close();
        return true;
    }
    else
    {
       return false;
    }
}
bool FileInOut::outputFile(QString path, QVector<Geometry> *vectorGeom)
{
    QFile file1(path);
    QJsonObject jsObj;
    QJsonDocument doc;
    QString strJs;
    QJsonArray jsArray;
    Geometry geom;
    if(file1.open(QIODevice::ReadOnly))
    {
        strJs = file1.readAll();
        doc= QJsonDocument::fromJson(strJs.toUtf8());
        jsObj = doc.object();
        jsArray = jsObj["Operations"].toArray();
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
