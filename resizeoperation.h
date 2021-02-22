#ifndef RESIZEOPERATION_H
#define RESIZEOPERATION_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

class ResizeOperation : public QGraphicsItem
{
public:
    ResizeOperation(QGraphicsItem *parent);
    double x;
    double y;
private:
    virtual QRectF boundingRect() const override;
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // RESIZEOPERATION_H