#ifndef FILEINOUT_H
#define FILEINOUT_H

#include "operation.h"
#include "Structures.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

class FileInOut
{
public:
    FileInOut();
    ~FileInOut();
    bool inputJSONFile(QVector<Operation*> operations,QString path);                    //Запись в JSON файл
    bool outputJSONFile(QString path,QVector<Geometry> *vectorGeom);                    //Считывание из JSON файла
    bool outputNamesOperation(QString path, QVector<Geometry> *vectorGeom);
};

#endif // FILEINOUT_H
