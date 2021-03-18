#include "fileinout.h"

FileInOut::FileInOut()
{

}
FileInOut::~FileInOut()
{
}
bool FileInOut::inputJSONFile(QVector<Operation *> operations, QString path)
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
            if(ops->inQueue)
            {
                jsObj.insert("x",0);
                jsObj.insert("y",0);
            }
            else
            {
                jsObj.insert("x",ops->pos().x());
                jsObj.insert("y",ops->pos().y());
            }
            jsObj.insert("width",ops->width/Operation::getCoef());
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
bool FileInOut::outputJSONFile(QString path, QVector<Geometry> *vectorGeom)
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
bool FileInOut::outputFile(QString path, QStringList *list)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QString textLine;
        while (!file.atEnd()) {
            textLine = file.readLine();
            list->push_back(textLine.left(textLine.indexOf(':')));
        }
        file.close();
        return true;
    }
    return false;
}
bool FileInOut::outputCurrentEquipment(QString path, QStringList *list, const QString &arg1)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QString textLine;
        while(!file.atEnd())
        {
            textLine = file.readLine();
            if(textLine.indexOf(arg1) != -1)
            {
                textLine = textLine.remove(0,textLine.indexOf(':')+2);
                list->append(textLine.split(','));
                break;
            }
        }
        file.close();
        return true;
    }
    return false;
}
