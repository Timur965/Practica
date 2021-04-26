#include "operation.h"

int Operation::id = 1;
int Operation::coef=6;
Operation::Operation(QString name, double x, double y, double width, double interval, bool dynamic, bool inQueue)
{
    if(name.isEmpty() && dynamic)
        this->name = "Операция" + QString::number(id);
    else
        this->name = "Рк" + QString::number(id);
    if(!name.isEmpty())
        this->name = name;
    if(dynamic)
        ro = new ResizeOperation(this);

    this->x=0;
    this->y=0;
    if(!dynamic)
    {
        this->width=1;
        this->interval = interval;
    }
    else
    {
        this->width=width;
        this->interval = 0;
    }
    this->dynamic = dynamic;
    transformOperation(this->name,x,y,this->width,this->interval);

    this->inQueue = inQueue;
    id++;
    setFlag(QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
}
QRectF Operation::boundingRect() const                                                                                                          //Определяем внешние границы прямоугольника
{
    return QRectF(x, y-height, width, height);
}
void Operation::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)                                                           //Отрисовываем операцию
{
    QRectF rect = QRectF(x, y-height, width, height);
    QBrush brush;
    if(press)                                                                                                                                   //Устанавливаем цвет операции
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
        painter->drawLine(x-2,y+2,width+2+x,y+2);                                                                                               //Нижняя линия
        painter->drawLine(x-2,y-height-2,width+2+x,y-height-2);                                                                                 //Верхняя линия
        painter->drawLine(x-2,y-height-2,x-2,y+2);                                                                                              //Левая линия
        painter->drawLine(x+width+2,y-height-2,x+width+2,y+2);                                                                                  //Правая линия
        if(dynamic)
        {
            painter->drawText(0,rect.bottomLeft().y()+40,rect.width()+35,rect.height(),Qt::AlignLeft,"Tн"+name.left(2)+name.right(1));          //Отрисовываем Тн
            if(this->width > 75)
                painter->drawText(-3,rect.bottomLeft().y()+40,rect.width(),rect.height(),Qt::AlignRight,"Tк"+name.left(2)+name.right(1));       //Отрисовываем Тк
            else
            {
                if(this->width > 50 && this->width < 75)
                    painter->drawText(-3,rect.bottomLeft().y()+50,rect.width(),rect.height(),Qt::AlignRight,"Tк"+name.left(2)+name.right(1));
                else
                    painter->drawText(-3,rect.bottomLeft().y()+50,rect.width()+35,rect.height(),Qt::AlignRight,"Tк"+name.left(2)+name.right(1));
            }
        }
        else
        {
            painter->drawText(0,rect.bottomLeft().y()+40,rect.width()+35,rect.height(),Qt::AlignLeft,name);
        }
    }
    start = (sceneWidth/2 + this->pos().x()-coordStart)/coef;                                                                                   //Начало операции
    end = ((sceneWidth/2 + this->pos().x()+this->width)-coordStart)/coef;                                                                       //Конец операции
    if(move && dynamic)
    {
        painter->drawText(0,rect.topLeft().y()-18,rect.width()+75,rect.height(),Qt::AlignLeft,"Начало: "+QString::number(start,'f',1));         //Отрисовываем начало операции
        painter->setPen(Qt::darkBlue);
        painter->drawText(0,rect.bottomLeft().y(),rect.width()+75,rect.height(),Qt::AlignLeft,"Конец: "+QString::number(end,'f',1));            //Отрисовываем конец операции
        painter->setPen(Qt::black);
    }
    if(inQueue)
    {
        brush=Qt::magenta;
    }
    if(inQueue && !dynamic)
    {
        brush=Qt::darkMagenta;
    }
    painter->fillRect(rect, brush);                                                                                                             //Отрисовываем прямоугольник
    if(dynamic)
    {
        painter->drawText(rect.x(), rect.y(),rect.width(),rect.height(),Qt::AlignCenter,name);                                                  //Пишем название операции
        ro->installSceneEventFilter(this);                                                                                                      //Устанавливаем фильтр событий для ro на операцию
    }
}
void Operation::setSceneSize(double widthView, double heightView)                                                                               //Метод для получения размеров сцены
{
    sceneWidth = widthView;
    sceneHeight = heightView;
}
void Operation::setCoordStart(int coord)                                                                                                        //Метод для получения начала сцены
{
    coordStart = coord;
}
int Operation::getCoef()                                                                                                                        //Метод для получения коэффицента ширины операции
{
    return coef;
}
void Operation::transformOperation(QString name, double x, double y, double width, double interval)                                                //Изменение операции
{
    this->setFocus();
    this->setPos(x,y);                                                                                                                          //Устанавливаем операцию на новые координаты
    this->name = name;                                                                                                                          //Устанавливаем новое имя операции
    if(this->dynamic)
    {
        this->width=width*coef;                                                                                                                 //Задаём новую ширину операции
        ro->x = this->width;                                                                                                                    //Устанавливаем новое начало для ro
        ro->y = -this->height;                                                                                                                  //Устанавливаем новую координату по у для ro
        ro->height = this->height;                                                                                                              //Устанавливаем новую высоту для ro
        ro->width=3;                                                                                                                            //Устанавливаем ширину для ro
        emit changeOperation();                                                                                                                 //Вызываем сигнал
    }
    else
    {
        this->width=1*coef;
        this->interval=interval*coef;
    }
    this->clearFocus();
}
void Operation::mousePressEvent(QGraphicsSceneMouseEvent *event)                                                                                //Нажатие на операцию
{
    prx=this->pos().x();                                                                                                                        //Сохраняем предыдущию координату по х
    pry=this->pos().y();                                                                                                                        //Сохраняем предыдущию координату по у
    this->setFocus();
    press=true;
    move=true;
    event->widget()->update();                                                                                                                  //Перерисовываем виджет
}

void Operation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)                                                                                 //Движение операции
{
    if(this->dynamic)
    {
        if(this->collidingItems().count() == 1)                                                                                                 //Проверка на столкновение
        {
            this->setPos(mapToScene(event->pos()+QPointF(-width/2,height/2)));                                                                  //Перетаскивание операции по сцене
            move=true;
            event->widget()->update();                                                                                                          //Перерисовываем виджет
        }
    }
    else
    {
        if(this->collidingItems().count() == 0)
        {
            this->setPos(mapToScene(event->pos()+QPointF(-width/2,height/2)));                                                                  //Перетаскивание операции по сцене
            move=true;
            event->widget()->update();                                                                                                          //Перерисовываем виджет
        }
    }
}
void Operation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)                                                                              //Отпустили операцию
{
    if(this->pos().x() < -sceneWidth/2+58 || this->pos().x()+width > sceneWidth/2+60 || this->pos().y() < -sceneHeight/2+80 || this->pos().y() > sceneHeight/2 - 48)
    {                                                                                                                                           //Если операция вышла за границы сцены возвращяем её на предидущии координаты
       this->setPos(prx,pry);
    }
    if(this->collidingItems().count() > 1 && this->dynamic)                                                                                     //Если операция столкнулась с другой операцией возвращаем её на предыдущии координаты для динамической операции
    {
        this->setPos(prx,pry);
    }
    if(this->collidingItems().count() > 0 && !this->dynamic)                                                                                    //Если операция столкнулась с другой операцией возвращаем её на предыдущии координаты для статической операции
    {
        this->setPos(prx,pry);
    }
    this->clearFocus();
    press=false;
    move=false;
    event->widget()->update();                                                                                                                  //Перерисовываем виджет
}
void Operation::hoverEnterEvent(QGraphicsSceneHoverEvent *event)                                                                                //Навелись на операцию
{
    hover = true;
    event->widget()->update();                                                                                                                  //Перерисовываем виджет
}
void Operation::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)                                                                                //Убрали наведение с операции
{
    hover=false;
    event->widget()->update();                                                                                                                  //Перерисовываем виджет
}
bool Operation::sceneEventFilter(QGraphicsItem *watched, QEvent *event)                                                                         //Фильтруем события для элемента watched. event это фильтруемое событие
{
    QGraphicsSceneMouseEvent *me = dynamic_cast<QGraphicsSceneMouseEvent*>(event);                                                              //Преобразуем QEvent в QGraphicsSceneMouseEvent*
    if(me==NULL) return false;
    if(watched==ro)                                                                                                                             //Если просматриваемый объект = ro
    {
        if(inQueue)
        {
            if(me->pos().x() >= coef && me->pos().x() <= 360*coef  && this->dynamic)
            {
                this->width = me->pos().x();                                                                                                    //Задаём новую ширину операции
                ro->x = this->width;                                                                                                            //Устанавливаем новую координату по х для ro
                this->update();                                                                                                                 //Перерисовываем операцию
                ro->update();                                                                                                                   //Перерисовываем ro
                me->widget()->update();                                                                                                         //Перерисовываем виджет
                emit changeOperation();                                                                                                         //Вызываем сигнал
                move=true;
            }
        }
        else
        {
            if(me->pos().x() >= coef && me->pos().x() <= 360  && this->dynamic)
            {
                this->width = me->pos().x();                                                                                                    //Задаём новую ширину операции
                ro->x = this->width;                                                                                                            //Устанавливаем новую координату по х для ro
                this->update();                                                                                                                 //Перерисовываем операцию
                ro->update();                                                                                                                   //Перерисовываем ro
                me->widget()->update();                                                                                                         //Перерисовываем виджет
                emit changeOperation();                                                                                                         //Вызываем сигнал
                move=true;
            }
        }
        if(me->type() == QEvent::GraphicsSceneMouseRelease)                                                                                     //Если мы отпустили клавишу мыши
        {
            emit releaseResizeOperation();                                                                                                      //Вызываем сигнал
            move=false;
        }
    }
    return true;
}
