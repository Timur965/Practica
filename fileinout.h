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
    bool inputFile(QVector<Operation*> operations,QString path);
    bool outputFile(QString path,QVector<Geometry> *vectorGeom);
};

#endif // FILEINOUT_H
