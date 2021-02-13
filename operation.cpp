#include "operation.h"

Operation::Operation()
{

}
Operation::Operation(double x, double y, double width, double height)
{
    static int id = 1;
    this->name = "Operation" + QString::number(id);
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    id++;
}
QRectF Operation::boundingRect() const
{
    return QRectF(x, y, width, height);
}
void Operation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF rect = QRectF(x, y, width, height);
    painter->fillRect(rect, Qt::red);
    painter->drawText(rect.x(), rect.y(),rect.width(),rect.height(),Qt::AlignCenter,name);
}
void Operation::transformOperation(double x, double y, double width, double height)
{
    setPos(x,y);
    this->width=width;
    this->height=height;
}
void Operation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    prx=this->pos().x();
    pry=this->pos().y();
}

void Operation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->collidingItems().isEmpty())
    {
        this->setPos(mapToScene(event->pos()+QPointF(-width/2,-height/2)));
    }
}

void Operation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->pos().x() < (event->widget()->width()/2 - event->widget()->width())+55 || this->pos().x() > event->widget()->width()/2 - 35 || this->pos().y() < (event->widget()->height()/2 - event->widget()->height())+20 || this->pos().y() > event->widget()->height()/2 - 55)
    {
       this->setPos(prx,pry);
    }
    if(!this->collidingItems().isEmpty())
    {
        this->setPos(prx,pry);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
