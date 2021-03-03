#ifndef FILEINOUT_H
#define FILEINOUT_H

#include "operation.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

struct Geometry
{
    double x;
    double y;
    double width;
    double height;
};

class FileInOut
{
public:
    FileInOut();
    ~FileInOut();
    void inputFile(QVector<Operation*> operations);
    QVector<Geometry> outputFile();
private:
    Operation *op = nullptr;
};

#endif // FILEINOUT_H
