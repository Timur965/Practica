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
    bool inputJSONFile(QVector<Operation*> operations,QString path);
    bool outputJSONFile(QString path,QVector<Geometry> *vectorGeom);
    bool outputFile(QString path,QStringList *list);
    bool outputCurrentEquipment(QString path,QStringList *list, const QString &arg1);
};

#endif // FILEINOUT_H
