#include "operation.h"

Operation::Operation()
{
}
int Operation::id = 1;
Operation::Operation(double x, double y, double width, double height)
{
    this->name = "Operation" + QString::number(id);
    ro = new ResizeOperation(this);
    ro->x = width;
    ro->y = height;
    ro->width=7;
    ro->height = height;

    this->x=0;
    this->y=0;
    this->width=width;
    this->height=height;
    transformOperation(x,y,this->width,this->height);

    inQueue = false;
    id++;
    setFlag(QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
}
QRectF Operation::boundingRect() const                                                              //Определяем внешние границы прямоугольника
{
    return QRectF(x, y-height, width, height);
}
void Operation::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)               //Отрисовываем операцию
{
    QRectF rect = QRectF(x, y-height, width, height);
    QBrush brush;
    if(press)
    {
        brush=Qt::yellow;
    }
    else
    {
        brush=Qt::red;
    }
    if(hover)
    {
        painter->drawLine(x-2,y+2,width+2+x,y+2);                //Нижняя
        painter->drawLine(x-2,y-height-2,width+2+x,y-height-2);  //Верхняя
        painter->drawLine(x-2,y-height-2,x-2,y+2);               //Левая
        painter->drawLine(x+width+2,y-height-2,x+width+2,y+2);   //Правая
    }
    start = sceneWidth/2 + this->pos().x()-59;
    end = (sceneWidth/2 + this->pos().x()+this->width)-59;
    if(move)
    {
        painter->drawText(0,rect.y()-height/2-20,rect.width()+30,rect.height(),Qt::AlignCenter,QString::number(start));
        painter->drawText(0,rect.y()+height/2+20,rect.width()+30,rect.height(),Qt::AlignCenter,QString::number(end));
    }
    if(inQueue)
        brush=Qt::magenta;
    painter->fillRect(rect, brush);                                                                 //Отрисовываем прямоугольник красного цвета
    painter->drawText(rect.x(), rect.y(),rect.width(),rect.height(),Qt::AlignCenter,name);          //Пишем название операции
    ro->installSceneEventFilter(this);
}
void Operation::setSceneSize(double widthView, double heightView)
{
    sceneWidth = widthView;
    sceneHeight = heightView;
}
void Operation::transformOperation(double x, double y, double width, double height)                 //Изменение операции
{
    this->setFocus();
    this->setPos(x,y);                                                                              //Устанавливаем операцию на новые координаты
    this->width=width;                                                                              //Задаём новую ширину операции
    this->height=height;                                                                            //Задаём новую высоту операции
    ro->x = width;
    if(y>0)
    {
        ro->y = -height;
        ro->height = height;
    }
    else
    {
        ro->y = this->y - height;
        ro->height = height;
    }
    ro->width=7;
    emit changeOperation();
    this->clearFocus();
}
void Operation::mousePressEvent(QGraphicsSceneMouseEvent *event)                                    //Нажатие на операцию
{   
    Q_UNUSED(event);
    prx=this->pos().x();                                                                            //Сохраняем предыдущию координату по х
    pry=this->pos().y();                                                                            //Сохраняем предыдущию координату по у
    this->setFocus();
    press=true;
    move=true;
    event->widget()->update();
}

void Operation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)                                     //Движение операции
{
    if(this->collidingItems().count() == 1)                                                         //Проверка на столкновение
    {
        this->setPos(mapToScene(event->pos()+QPointF(-width/2,height/2)));                //Перетаскивание операции по сцене
        move=true;
        event->widget()->update();
    }
}

void Operation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)                                  //Отпустили операцию
{
    if(this->pos().x() < -sceneWidth/2+58 || this->pos().x()+width > sceneWidth/2+60 || this->pos().y() < -sceneHeight/2+80 || this->pos().y() > sceneHeight/2 - 28)
    {                                                                                               //Если операция вышла за границы сцены возвращяем её на предидущии координаты
       this->setPos(prx,pry);
    }
    if(this->collidingItems().count() != 1)                                                         //Если операция столкнулась с другой операцией возвращяем её на предыдущии координаты
    {
        this->setPos(prx,pry);
    }
    this->clearFocus();
    press=false;
    move=false;
    event->widget()->update();
    QGraphicsItem::mouseReleaseEvent(event);
}
void Operation::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    hover = true;
    event->widget()->update();
}
void Operation::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hover=false;
    event->widget()->update();
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
            ro->x = this->x+this->width;
            this->update();
            ro->update();
            me->widget()->update();
            emit changeOperation();
        }
        if(me->type() == QEvent::GraphicsSceneMouseRelease)
        {
            emit releaseResizeOperation();
        }
    }
    return true;
}
