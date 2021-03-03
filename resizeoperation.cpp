#include "resizeoperation.h"

ResizeOperation::ResizeOperation(QGraphicsItem *parent)
{
    setParentItem(parent);
}
QRectF ResizeOperation::boundingRect() const
{
    return QRectF(x-width,y,width,height);
}
void ResizeOperation::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rect (x-width,y,width,height);
    painter->fillRect(rect,Qt::black);
}
