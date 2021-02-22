#include "mygraphicscene.h"

MyGraphicScene::MyGraphicScene()
{
}
MyGraphicScene::MyGraphicScene(QObject *parent) : QGraphicsScene(parent)
{
}
MyGraphicScene::~MyGraphicScene()
{
    if(op != nullptr)
        delete op;
    operations.clear();
    operations.squeeze();
}
QString MyGraphicScene::getName()
{
    return op->name;
}
bool MyGraphicScene::addOperations(double x, double y, double width, double height)                     //Добавление операции на сцену
{
    if(this->items(x,y-height,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty()) //Проверяем не сталкивается ли новый объект со старыми
    {
        op = new Operation(x,y,width,height);                                                           //Инициализируем новую операцию
        connect(op,&Operation::changeOperation,this,&MyGraphicScene::processingChange);
        operations.push_back(op);                                                                       //Добавляем указатель на операцию в вектор
        this->addItem(op);                                                                              //Добавляем операцию на сцену
        this->update();                                                                                 //Обновляем сцену                                                                              //Возвращаем название операции
        return true;
    }
    return false;
}
bool MyGraphicScene::updateOperations(int index,double x, double y, double width, double height)        //Изменение операции на сцене
{
    if(this->items(x,y,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty() || operations.at(index)->pos() == QPointF(x,y))
    {                                                                                                   //Проверяем не сталкивается ли новый объект со старыми
        operations.at(index)->transformOperation(x,y,width,height);                                     //Изменяем объект на сцене
        connect(operations.at(index),&Operation::changeOperation,this,&MyGraphicScene::processingChange);
        this->update();                                                                                 //Обновляем сцену
        return true;
    }
    return false;
}
void MyGraphicScene::deleteOperations(int index)                                                        //Удаление операции со сцены
{
    this->removeItem(operations[index]);                                                                //Удаленяем операцию со сцены
    operations.remove(index);                                                                           //Удаляем указатель на операцию из вектора
    this->update();                                                                                     //Обновляем сцену
}
void MyGraphicScene::drawBackground(QPainter *painter, const QRectF &rect)                              //Отрисовка заднего фона
{
    Q_UNUSED(rect);
    painter->setPen(QPen(Qt::black));                                                                   //Устанавливаем цвет пера
    painter->drawLine(-width/2,height/2-15,width,height/2-15);                                          //Рисуем линию на весь размер представления
    for(int i=-width/2-1;i<width/2+30;i=i+30)                                                              //Рисуем вертикальные линии
    {
        painter->drawLine(i,height/2-25,i,height/2-5);
    }
    int x=0;
    for(int i=-width/2+60;i<width/2+60;i=i+60)                                                             //Рисуем значения
    {
        if(i<=width/2+60)
            painter->drawText(i,height/2+5,QString::number(x));
        else
            painter->drawText(i/2+15,height/2+5,QString::number(x));
        x +=60;
    }
    painter->drawLine(width/2+40,height/2-30,width/2+57,height/2-15);                                   //Рисуем верхнюю стороны стрелки
    painter->drawLine(width/2+40,height/2,width/2+57,height/2-15);                                      //Рисуем нижнюю стороны стрелки
}
double MyGraphicScene::maxHeight()
{
    double max=operations.at(0)->height;
    foreach(Operation *ops, operations)
    {
        if(ops->height>max)
        {
            max=ops->height;
        }
    }
    return max;
}
bool MyGraphicScene::createQueue()
{
    if(operations.count() != 0)
    {
        double x = -width/2+58;
        double y = height/2-30;
        double sum = 0;
        foreach(Operation *ops, operations)
        {
            if(x+ops->width <= width/2 + 60)
            {
                ops->setPos(x,y);
                x=x+ops->width;
            }
            else
            {
                sum += maxHeight();
                x = -width/2+58;
                y = (height/2-30)-sum;
                ops->setPos(x,y);
                x=x+ops->width;
            }
            ops->inQueue = true;
        }
        return true;
    }
    return false;
}
void MyGraphicScene::processingChange()
{
    Operation *focusOperation;
    focusOperation = static_cast<Operation*>(this->focusItem());
    foreach(Operation *ops, operations)
    {
        if(focusOperation == ops && ops->inQueue)
        {
                createQueue();
                break;
        }
    }
}
