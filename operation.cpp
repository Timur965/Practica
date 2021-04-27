#include "operation.h"

int Operation::id = 1;
int Operation::coef = 30;
Operation::Operation(QString name, double x, double y, double width, double interval, bool dynamic)
{
    if(name.isEmpty() && dynamic)
        this->name = "Операция" + QString::number(id);
    else
        this->name = "Рк" + QString::number(id);
    if(!name.isEmpty())
        this->name = name;

    this->x=0;
    this->y=0;
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
    transformOperation(this->name,x,y,this->width,this->interval);

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
    QFont font = painter->font();
    font.setPointSize(10);
    painter->setFont(font);
    start = (sceneWidth/2 + this->pos().x()-coordStart)/coef;                                                                                   //Начало операции
    end = ((sceneWidth/2 + this->pos().x()+this->width)-coordStart)/coef;                                                                       //Конец операции
    if(hover)
    {
        painter->drawLine(x-2,y+2,width+2+x,y+2);                                                                                               //Нижняя линия
        painter->drawLine(x-2,y-height-2,width+2+x,y-height-2);                                                                                 //Верхняя линия
        painter->drawLine(x-2,y-height-2,x-2,y+2);                                                                                              //Левая линия
        painter->drawLine(x+width+2,y-height-2,x+width+2,y+2);                                                                                  //Правая линия
        if(dynamic)
        {
            painter->drawText(0,rect.topLeft().y()-25,rect.width()+100,rect.height(),Qt::AlignLeft,"Начало: "+QString::number(start,'f',1));    //Отрисовываем начало операции
            painter->drawText(0,rect.bottomLeft().y()+35,rect.width()+100,rect.height(),Qt::AlignLeft,"Конец: "+QString::number(end,'f',1));    //Отрисовываем конец операции
        }
    }
    if(dynamic)
    {
        painter->drawText(0,rect.topLeft().y()-50,rect.width()+35,rect.height(),Qt::AlignLeft,"Tн"+name.left(2)+name.right(1));                 //Отрисовываем Тн
        if(this->width > 75)
            painter->drawText(-3,rect.topLeft().y()-50,rect.width(),rect.height(),Qt::AlignRight,"Tк"+name.left(2)+name.right(1));              //Отрисовываем Тк
        else
        {
            if(this->width > 50 && this->width < 75)
                painter->drawText(-3,rect.topLeft().y()-40,rect.width(),rect.height(),Qt::AlignRight,"Tк"+name.left(2)+name.right(1));
            else
                painter->drawText(-3,rect.topLeft().y()-40,rect.width()+35,rect.height(),Qt::AlignRight,"Tк"+name.left(2)+name.right(1));
        }
    }
    else
    {
        if(name.length() > 2)
            painter->drawText(0,rect.topLeft().y()-70,rect.width()+35,rect.height(),Qt::AlignLeft,name.left(2)+name.right(1));
        else
            painter->drawText(0,rect.topLeft().y()-70,rect.width()+35,rect.height(),Qt::AlignLeft,name);
    }
    if(dynamic)
    {
        painter->fillRect(rect, Qt::cyan);                                                                                                      //Отрисовываем прямоугольник
        painter->drawText(rect.x(), rect.y(),rect.width(),rect.height(),Qt::AlignCenter,name);                                                  //Пишем название операции
    }
    else
    {
        painter->fillRect(rect, Qt::black);                                                                                                      //Отрисовываем прямоугольник
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
void Operation::transformOperation(QString name, double x, double y, double width, double interval)                                             //Изменение операции
{
    this->setFocus();
    this->setPos(x,y);                                                                                                                          //Устанавливаем операцию на новые координаты
    this->name = name;                                                                                                                          //Устанавливаем новое имя операции
    if(this->dynamic)
    {
        this->width=width*coef;                                                                                                                 //Задаём новую ширину операции
        this->interval=interval*coef;
        emit changeOperation();                                                                                                                 //Вызываем сигнал
    }
    else
    {
        this->width=1;
        this->interval=interval*coef;
    }
    this->clearFocus();
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
