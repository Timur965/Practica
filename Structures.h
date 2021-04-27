#ifndef STRUCTURES_H
#define STRUCTURES_H

#include<QString>

struct Geometry
{
    QString name;           //Название операции
    double x;               //Координата х операции
    double y;               //Координата у операции
    double width;           //Ширина операции
    double interval;        //Высота операции
    bool dynamic;           //Динамическая операция
};

#endif // STRUCTURES_H
