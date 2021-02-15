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
QRectF Operation::boundingRect() const                                                              //Определяем внешние границы прямоугольника
{
    return QRectF(x, y, width, height);
}
void Operation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)   //Отрисовываем операцию
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF rect = QRectF(x, y, width, height);
    painter->fillRect(rect, Qt::red);                                                               //Отрисовываем прямоугольник красного цвета
    painter->drawText(rect.x(), rect.y(),rect.width(),rect.height(),Qt::AlignCenter,name);          //Пишем название операции
}
void Operation::transformOperation(double x, double y, double width, double height)                 //Изменение операции
{
    this->setPos(x,y);                                                                              //Устанавливаем операцию на новые координаты
    this->width=width;                                                                              //Задаём новую ширину операции
    this->height=height;                                                                            //Задаём новую высоту операции
}
void Operation::mousePressEvent(QGraphicsSceneMouseEvent *event)                                    //Нажатие на операцию
{
    Q_UNUSED(event);
    prx=this->pos().x();                                                                            //Сохраняем предыдущию координату по х
    pry=this->pos().y();                                                                            //Сохраняем предыдущию координату по у
}

void Operation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)                                     //Движение операции
{
    if(this->collidingItems().isEmpty())                                                            //Проверка на столкновение
    {
        this->setPos(mapToScene(event->pos()+QPointF(-width/2,-height/2)));                         //Перетаскивание операции по сцене
    }
}

void Operation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)                                  //Отпустили операцию
{
    if(this->pos().x() < (event->widget()->width()/2 - event->widget()->width())+55 || this->pos().x() > event->widget()->width()/2 - 35 || this->pos().y() < (event->widget()->height()/2 - event->widget()->height())+20 || this->pos().y() > event->widget()->height()/2 - 55)
    {                                                                                               //Если операция вышла за границы сцены возвращяем её на предидущии координаты
       this->setPos(prx,pry);
    }
    if(!this->collidingItems().isEmpty())                                                           //Если операция столкнулась с другой операцией возвращяем её на предыдущии координаты
    {
        this->setPos(prx,pry);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
