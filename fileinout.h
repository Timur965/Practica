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
    bool outputFile(QString path,QStringList *list);                                    //Считывание из обычного файла
    bool outputCurrentEquipment(QString path,QStringList *list, const QString &arg1);   //Считывание из файла текушей аппаратуры
};

#endif // FILEINOUT_H
