#include "resizeoperation.h"

ResizeOperation::ResizeOperation(QGraphicsItem *parent)
{
    setParentItem(parent);
}
QRectF ResizeOperation::boundingRect() const
{
    return QRectF(x-7,-y,7,y);
}
void ResizeOperation::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rect (x-7,-y,7,y);
    painter->fillRect(rect,Qt::black);
}
