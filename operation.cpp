#include "operation.h"

int Operation::id = 0;
int Operation::coef = 30;
Operation::Operation(QString name, QString reduction, double width, double interval, bool dynamic)
{
        this->name = name;
        this->reduction = reduction;

    if(!dynamic)
    {
        this->width=1;
    }
    else
    {
        this->width=width;
    }
    this->interval = interval;
    this->dynamic = dynamic;
    transformOperation(this->name,this->reduction,this->width,this->interval);

    setFlag(QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
}
QRectF Operation::boundingRect() const                                                                                                                 //Определяем внешние границы прямоугольника
{
    return QRectF(interval, -height, width, height);
}
void Operation::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)                                                                  //Отрисовываем операцию
{
    QRectF rect = QRectF(interval, -height, width, height);
    QFont font = painter->font();
    font.setPointSize(10);
    painter->setFont(font);
    start = (sceneWidth/2 + interval + this->pos().x()-coordStart)/coef;                                                                                          //Начало операции
    end = ((sceneWidth/2 + interval + this->pos().x()+this->width)-coordStart)/coef;                                                                              //Конец операции
    if(hover)
    {
        painter->drawLine(interval-2,2,interval+width+2,2);                                                                                            //Нижняя линия
        painter->drawLine(interval-2,-height-2,interval+width+2,-height-2);                                                                            //Верхняя линия
        painter->drawLine(interval-2,-height-2,interval-2,2);                                                                                          //Левая линия
        painter->drawLine(interval+width+2,-height-2,interval+width+2,2);                                                                              //Правая линия
        if(dynamic)
        {
            painter->drawText(interval,rect.topLeft().y()-25,rect.width()+100,rect.height(),Qt::AlignLeft,"Начало: "+QString::number(start,'f',1));    //Отрисовываем начало операции
            painter->drawText(interval,rect.bottomLeft().y(),rect.width()+100,rect.height(),Qt::AlignLeft,"Конец: "+QString::number(end,'f',1));       //Отрисовываем конец операции
        }
    }
    if(dynamic)
    {
        painter->rotate(-90);
        painter->drawText(rect.topLeft().y()+105,interval,rect.width()+35,rect.height(),Qt::AlignLeft,"Тн"+reduction);                                   //Отрисовываем Тн
        painter->drawText(rect.topLeft().y()+105,interval+rect.width()-15,rect.width()+15,rect.height(),Qt::AlignLeft,"Тк"+reduction);                     //Отрисовываем Тк
        painter->rotate(90);
    }
    else
    {
        painter->rotate(-90);
        painter->drawText(-rect.topLeft().y()+53,interval-5,rect.width()+50,rect.height(),Qt::AlignLeft,reduction);
        painter->rotate(90);
        //painter->drawText(interval,rect.topLeft().y()-70,rect.width()+50,rect.height(),Qt::AlignLeft,reduction);
    }
    if(dynamic)
    {
        painter->fillRect(rect, Qt::cyan);                                                                                                             //Отрисовываем прямоугольник
        painter->drawText(rect.x(), rect.y(),rect.width(),rect.height(),Qt::AlignCenter,name);                                                         //Пишем название операции
    }
    else
    {
        painter->fillRect(rect, Qt::black);                                                                                                            //Отрисовываем прямоугольник
    }
}
void Operation::setSceneSize(double widthView, double heightView)                                                                                      //Метод для получения размеров сцены
{
    sceneWidth = widthView;
    sceneHeight = heightView;
}
void Operation::setCoordStart(int coord)                                                                                                               //Метод для получения начала сцены
{
    coordStart = coord;
}
int Operation::getCoef()                                                                                                                               //Метод для получения коэффицента ширины операции
{
    return coef;
}
int Operation::getId()                                                                                                                               //Метод для получения коэффицента ширины операции
{
    return id;
}
void Operation::transformOperation(QString name, QString reduction, double width, double interval)                                                     //Изменение операции
{
    this->setFocus();
    this->name = name;                                                                                                                                 //Устанавливаем новое имя операции
    this->reduction = reduction;
    if(this->dynamic)
    {
        this->width=width*coef;                                                                                                                        //Задаём новую ширину операции
    }
    else
    {
        this->width=1;
    }
    this->interval=interval*coef;
    emit changeOperation();                                                                                                                            //Вызываем сигнал
    this->clearFocus();
}
void Operation::hoverEnterEvent(QGraphicsSceneHoverEvent *event)                                                                                       //Навелись на операцию
{
    hover = true;
    event->widget()->update();                                                                                                                         //Перерисовываем виджет
}
void Operation::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)                                                                                       //Убрали наведение с операции
{
    hover=false;
    event->widget()->update();                                                                                                                         //Перерисовываем виджет
}
