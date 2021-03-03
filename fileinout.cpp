#include "fileinout.h"

FileInOut::FileInOut()
{

}
FileInOut::~FileInOut()
{
    if(op != nullptr)
        delete op;
}
void FileInOut::inputFile(QVector<Operation *> operations)
{
    QFile file1("Operations.json");
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
            jsObj.insert("x",ops->pos().x());
            jsObj.insert("y",ops->pos().y());
            jsObj.insert("width",ops->width);
            jsObj.insert("height",ops->height);

            jsArray.push_back(jsObj);
        }
        jsObj1.insert("Operations",jsArray);
        doc = QJsonDocument(jsObj1);
        jsStr = doc.toJson(QJsonDocument::Compact);
        stream<<jsStr;
        file1.close();
    }
    else
    {
        qDebug()<<"Ошибка при открытии файла";
    }
}
QVector<Geometry> FileInOut::outputFile()
{
    QFile file1("Operations.json");
    QJsonObject jsObj;
    QJsonDocument doc;
    QString strJs;
    QJsonArray jsArray;
    QVector<Geometry> vectorGeom;
    Geometry geom;
    if(file1.open(QIODevice::ReadOnly))
    {
        strJs = file1.readAll();
        doc= QJsonDocument::fromJson(strJs.toUtf8());
        jsObj = doc.object();
        jsArray = jsObj["Operations"].toArray();
        foreach(QJsonValue js, jsArray)
        {
            geom.x = js["x"].toDouble();
            geom.y = js["y"].toDouble();
            geom.width = js["width"].toDouble();
            geom.height = js["height"].toDouble();
            vectorGeom.push_back(geom);
        }
        file1.close();
    }
    else
    {
        qDebug()<<"Ошибка при открытии файла";
    }
    return vectorGeom;
}
