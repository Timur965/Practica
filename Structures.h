#ifndef STRUCTURES_H
#define STRUCTURES_H

#include<QString>

struct Geometry
{
    QString name;
    double x;
    double y;
    double width;
    double height;
    bool inQueue;
    bool dynamic;
};

#endif // STRUCTURES_H
