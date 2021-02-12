#ifndef OPERATION_H
#define OPERATION_H

#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsView>

class Operation:public QGraphicsItem
{
public:
    Operation();
    Operation(double x=0, double y=0, double width=100, double height=30);
    QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    void transformOperation(double x, double y, double width, double height);
    double x;
    double y;
    double width;
    double height;
    QString name;
private:
    double prx;
    double pry;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // OPERATION_H
