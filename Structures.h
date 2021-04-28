#ifndef STRUCTURES_H
#define STRUCTURES_H

#include<QString>

struct Geometry
{
    QString name;           //Название операции
    QString reduction;
    double width;           //Ширина операции
    double interval;        //Высота операции
    bool dynamic;           //Динамическая операция
};

#endif // STRUCTURES_H
