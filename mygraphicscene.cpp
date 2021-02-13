#include "mygraphicscene.h"

MyGraphicScene::MyGraphicScene()
{

}
MyGraphicScene::MyGraphicScene(QObject *parent) : QGraphicsScene(parent)
{

}
MyGraphicScene::~MyGraphicScene()
{
    delete op;
}
QString MyGraphicScene::addOperations(double x, double y, double width, double height)                  //Добавление операции на сцену
{
    if(this->items(x,y,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty()) //Проверяем не сталкивается ли новый объект со старыми
    {
        op = new Operation(x,y,width,height);                                                           //Инициализируем новую операцию
        operations.push_back(op);                                                                       //Добавляем указатель на операцию в вектор
        this->addItem(op);                                                                              //Добавляем операцию на сцену
        this->update();                                                                                 //Обновляем сцену
        return op->name;                                                                                //Возвращаем название операции
    }
    else
    {
        return "Данные координаты заняты другим объектом.\nПодвиньте предыдущий объект.";
    }
}
void MyGraphicScene::updateOperations(int index,double x, double y, double width, double height)        //Изменение операции на сцене
{
    if(this->items(x,y,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty()) //Проверяем не сталкивается ли объект с другими объектами
    {
        operations[index]->transformOperation(x,y,width,height);                                        //Изменяем объект на сцене
        this->update();                                                                                 //Обновляем сцену
    }
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
    for(int i=-width/2-1;i<width/2;i=i+30)                                                              //Рисуем вертикальные линии
    {
        painter->drawLine(i,height/2-25,i,height/2-5);
    }
    int x=0;
    for(int i=-width/2+60;i<width/2;i=i+60)                                                             //Рисуем значения
    {
        if(i<=width/2)
            painter->drawText(i,height/2+5,QString::number(x));
        else
            painter->drawText(i/2+15,height/2+5,QString::number(x));
        x +=60;
    }
    painter->drawLine(width/2+40,height/2-30,width/2+57,height/2-15);                                   //Рисуем верхнюю стороны стрелки
    painter->drawLine(width/2+40,height/2,width/2+57,height/2-15);                                      //Рисуем нижнюю стороны стрелки
}
