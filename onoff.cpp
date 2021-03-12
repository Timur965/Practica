#include "onoff.h"

onOff::onOff()
{

}
QRectF onOff::boundingRect() const                                                              //Определяем внешние границы прямоугольника
{
    return QRectF(x, y-height, width, height);
}
void onOff::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)               //Отрисовываем операцию
{
    QRectF rect = QRectF(x, y-height, width, height);
    painter->drawText(rect.x(), rect.bottomLeft().y()+30,rect.width()+20,rect.height(),Qt::AlignCenter,name);      //Пишем название операции
    painter->fillRect(rect, Qt::black);                                                         //Отрисовываем прямоугольник красного цвета
}
