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
    ro = new ResizeOperation(this);
    ro->x = this->width;
    ro->y = this->height;
    inQueue = false;
    id++;
    setFlag(QGraphicsItem::ItemIsFocusable);
}
QRectF Operation::boundingRect() const                                                              //Определяем внешние границы прямоугольника
{
    return QRectF(x, y-height, width, height);
}
void Operation::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)               //Отрисовываем операцию
{
    QRectF rect = QRectF(x, y-height, width, height);
    painter->fillRect(rect, Qt::red);                                                               //Отрисовываем прямоугольник красного цвета
    painter->drawText(rect.x(), rect.y(),rect.width(),rect.height(),Qt::AlignCenter,name);          //Пишем название операции
    ro->installSceneEventFilter(this);
}
void Operation::transformOperation(double x, double y, double width, double height)                 //Изменение операции
{
    this->setFocus();
    this->setPos(x,y);                                                                              //Устанавливаем операцию на новые координаты
    this->width=width;                                                                              //Задаём новую ширину операции
    ro->x=width;
    this->height=height;                                                                            //Задаём новую высоту операции
    ro->y=height;
    emit changeOperation();
    this->clearFocus();
}
void Operation::mousePressEvent(QGraphicsSceneMouseEvent *event)                                    //Нажатие на операцию
{   
    Q_UNUSED(event);
    prx=this->pos().x();                                                                            //Сохраняем предыдущию координату по х
    pry=this->pos().y();                                                                            //Сохраняем предыдущию координату по у
    this->setFocus();
}

void Operation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)                                     //Движение операции
{
    if(this->collidingItems().count() == 1)                                                         //Проверка на столкновение
    {
        this->setPos(mapToScene(event->pos()+QPointF(-width/2,height/2)));                         //Перетаскивание операции по сцене
    }
}

void Operation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)                                  //Отпустили операцию
{
    double sceneWidth = event->widget()->width();
    double sceneHeight = event->widget()->height();
    if(this->pos().x() < -sceneWidth/2+56 || this->pos().x()+width > sceneWidth/2+60 || this->pos().y() < -sceneHeight/2+50 || this->pos().y() > sceneHeight/2 - 25)
    {                                                                                               //Если операция вышла за границы сцены возвращяем её на предидущии координаты
       this->setPos(prx,pry);
    }
    if(this->collidingItems().count() != 1)                                                         //Если операция столкнулась с другой операцией возвращяем её на предыдущии координаты
    {
        this->setPos(prx,pry);
    }
    this->clearFocus();
    QGraphicsItem::mouseReleaseEvent(event);
}
bool Operation::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    QGraphicsSceneMouseEvent *me = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if(me==NULL) return false;

    if(watched==ro)
    {
        if(me->pos().x() >= 10 && me->pos().x() <= 360)
        {
            this->width = me->pos().x();
            ro->x = this->width;
            this->update();
            ro->update();
            me->widget()->update();
            emit changeOperation();
        }
    }
    return true;
}
