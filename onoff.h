#ifndef ONOFF_H
#define ONOFF_H

#include<QGraphicsItem>
#include<QPainter>

class onOff: public QGraphicsItem
{
public:
    onOff();
    QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    double x;
    double y;
    double width;
    double height;
    QString name;
};

#endif // ONOFF_H
