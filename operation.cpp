#include "operation.h"

int Operation::id = 1;
int Operation::coef=6;
Operation::Operation(QString name, double x, double y, double width, double height, bool dynamic, bool inQueue)
{
    if(name.isEmpty())
        this->name = "Operation" + QString::number(id);
    else
        this->name = name;
    if(dynamic)
        ro = new ResizeOperation(this);

    this->x=0;
    this->y=0;
    this->width=width;
    this->height=height;
    this->dynamic = dynamic;
    transformOperation(this->name,x,y,this->width,this->height);

    this->inQueue = inQueue;
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
    if(!press && dynamic)
    {
        brush=Qt::red;
    }
    if(!press && !dynamic)
    {
        brush=Qt::green;
    }
    if(hover)
    {
        painter->drawLine(x-2,y+2,width+2+x,y+2);                //Нижняя
        painter->drawLine(x-2,y-height-2,width+2+x,y-height-2);  //Верхняя
        painter->drawLine(x-2,y-height-2,x-2,y+2);               //Левая
        painter->drawLine(x+width+2,y-height-2,x+width+2,y+2);   //Правая
    }
    start = (sceneWidth/2 + this->pos().x()-coordStart)/coef;
    end = ((sceneWidth/2 + this->pos().x()+this->width)-coordStart)/coef;
    if(move)
    {
        painter->drawText(0,rect.topLeft().y()-18,rect.width()+75,rect.height(),Qt::AlignLeft,"Начало: "+QString::number(start,'f',1));
        painter->setPen(Qt::darkBlue);
        painter->drawText(0,rect.bottomLeft().y(),rect.width()+75,rect.height(),Qt::AlignLeft,"Конец: "+QString::number(end,'f',1));
        painter->setPen(Qt::black);
    }
    if(inQueue)
    {
        brush=Qt::magenta;
        painter->drawText(0,rect.bottomLeft().y()+40,rect.width()+30,rect.height(),Qt::AlignLeft,"Tн"+name.left(2)+name.right(1));
        if(this->width < 75)
            painter->drawText(-3,rect.bottomLeft().y()+50,rect.width(),rect.height(),Qt::AlignRight,"Tк"+name.left(2)+name.right(1));
        else
            painter->drawText(-3,rect.bottomLeft().y()+40,rect.width(),rect.height(),Qt::AlignRight,"Tк"+name.left(2)+name.right(1));
    }
    if(inQueue && !dynamic)
        brush=Qt::darkMagenta;
    painter->fillRect(rect, brush);                                                                 //Отрисовываем прямоугольник красного цвета
    painter->drawText(rect.x(), rect.y(),rect.width(),rect.height(),Qt::AlignCenter,name);          //Пишем название операции
    if(this->dynamic)
        ro->installSceneEventFilter(this);
}
void Operation::setSceneSize(double widthView, double heightView)
{
    sceneWidth = widthView;
    sceneHeight = heightView;
}
void Operation::setCoordStart(int coord)
{
    coordStart = coord;
}
bool Operation::setMinWidth()
{
    if(!this->dynamic)
    {
        this->setFocus();
        this->width = 1*coef;
        this->update();
        emit changeOperation();
        this->clearFocus();
        return true;
    }
    return false;
}
bool Operation::setMaxWidth()
{
    if(!this->dynamic)
    {
        this->setFocus();
        this->width = 360*coef;
        this->update();
        emit changeOperation();
        this->clearFocus();
        return true;
    }
    return false;
}
int Operation::getCoef()
{
    return coef;
}
void Operation::transformOperation(QString name,double x, double y, double width, double height)        //Изменение операции
{
    this->setFocus();
    this->setPos(x,y);                                                                                  //Устанавливаем операцию на новые координаты
    this->name = name;
    if(this->dynamic)
    {
        this->width=width*coef;                                                                              //Задаём новую ширину операции
        this->height=height;                                                                            //Задаём новую высоту операции
        ro->x = this->width;
        if(y>0)
        {
            ro->y = -this->height;
            ro->height = this->height;
        }
        else
        {
            ro->y = this->y - this->height;
            ro->height = this->height;
        }
        ro->width=3;
        emit changeOperation();
    }
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
    if(this->dynamic)
    {
        if(this->collidingItems().count() == 1)                                                         //Проверка на столкновение
        {
            this->setPos(mapToScene(event->pos()+QPointF(-width/2,height/2)));                          //Перетаскивание операции по сцене
            move=true;
            event->widget()->update();
        }
    }
    else
    {
        if(this->collidingItems().count() == 0)
        {
            this->setPos(mapToScene(event->pos()+QPointF(-width/2,height/2)));                          //Перетаскивание операции по сцене
            move=true;
            event->widget()->update();
        }
    }
}

void Operation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)                                  //Отпустили операцию
{
    if(this->pos().x() < -sceneWidth/2+58 || this->pos().x()+width > sceneWidth/2+60 || this->pos().y() < -sceneHeight/2+80 || this->pos().y() > sceneHeight/2 - 28)
    {                                                                                               //Если операция вышла за границы сцены возвращяем её на предидущии координаты
       this->setPos(prx,pry);
    }
    if(this->collidingItems().count() > 1 && this->dynamic)                                         //Если операция столкнулась с другой операцией возвращяем её на предыдущии координаты
    {
        this->setPos(prx,pry);
    }
    if(this->collidingItems().count() > 0 && !this->dynamic)                                        //Если операция столкнулась с другой операцией возвращяем её на предыдущии координаты
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
        if(inQueue)
        {
            if(me->pos().x() >= coef && me->pos().x() <= 360*coef  && this->dynamic)
            {
                this->width = me->pos().x();
                ro->x = this->x+this->width;
                this->update();
                ro->update();
                me->widget()->update();
                emit changeOperation();
                move=true;
            }
        }
        else
        {
            if(me->pos().x() >= coef && me->pos().x() <= 360  && this->dynamic)
            {
                this->width = me->pos().x();
                ro->x = this->x+this->width;
                this->update();
                ro->update();
                me->widget()->update();
                emit changeOperation();
                move=true;
            }
        }
        if(me->type() == QEvent::GraphicsSceneMouseRelease)
        {
            emit releaseResizeOperation();
            move=false;
        }
    }
    return true;
}
