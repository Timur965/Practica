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
QString MyGraphicScene::addOperations(double x, double y, double width, double height)
{
    if(this->items(x,y,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty())
    {
        op = new Operation(x,y,width,height);
        operations.push_back(op);
        this->addItem(op);
        this->update();
        return op->name;
    }
    else
    {
        return "Данные координаты заняты другим объектом.\nПодвиньте предыдущий объект.";
    }
}
void MyGraphicScene::updateOperations(int index,double x, double y, double width, double height)
{
    if(this->items(x,y,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty())
    {
        operations[index]->transformOperation(x,y,width,height);
        this->update();
    }
}
void MyGraphicScene::deleteOperations(int index)
{
    this->removeItem(operations[index]);
    operations.remove(index);
    this->update();
}
void MyGraphicScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    painter->setPen(QPen(Qt::black));
    painter->drawLine(-width/2,height/2-15,width,height/2-15);
    for(int i=-width/2-1;i<width/2;i=i+30)
    {
        painter->drawLine(i,height/2-25,i,height/2-5);
    }
    int x=0;
    for(int i=-width/2+60;i<width/2;i=i+60)
    {
        if(i<=width/2)
            painter->drawText(i,height/2+5,QString::number(x));
        else
            painter->drawText(i/2+15,height/2+5,QString::number(x));
        x +=60;
    }
    painter->drawLine(width/2+40,height/2-30,width/2+57,height/2-15);
    painter->drawLine(width/2+40,height/2,width/2+57,height/2-15);
}
