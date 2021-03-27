#ifndef ONOFF_H
#define ONOFF_H

#include<QGraphicsItem>
#include<QPainter>

class onOff: public QGraphicsItem
{
public:
    onOff();
    QRectF boundingRect() const override;                                                           //Метод который определяет внешние границы элемента в виде прямоугольника
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;    //Метод для отрисовки
    double x;
    double y;
    double width;
    double height;
    QString name;
};

#endif // ONOFF_H
