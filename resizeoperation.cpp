#include "resizeoperation.h"

ResizeOperation::ResizeOperation(QGraphicsItem *parent)
{
    setParentItem(parent);                                                                      //Устанавливаем родителя элемента
}
QRectF ResizeOperation::boundingRect() const                                                    //Определяем внешние границы прямоугольника
{
    return QRectF(x-width,y,width,height);
}
void ResizeOperation::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)    //Метод для отрисовки
{
    QRectF rect (x-width,y,width,height);
    painter->fillRect(rect,Qt::black);                                                          //Отрисовываем прямоугольник
}
