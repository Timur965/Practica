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
    qDeleteAll(operations);
    listOperations.clear();
    qDeleteAll(listOperations);
    if(fileInOut != nullptr)
        delete fileInOut;
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
        connect(op,&Operation::releaseResizeOperation,this,&MyGraphicScene::processingRelease);
        operations.push_back(op);                                                                       //Добавляем указатель на операцию в вектор
        op->setSceneSize(this->width,this->height);
        this->addItem(op);                                                                              //Добавляем операцию на сцену
        this->update();                                                                                 //Обновляем сцену                                                                              //Возвращаем название операции
        return true;
    }
    return false;
}
bool MyGraphicScene::updateOperations(int index,double x, double y, double width, double height)        //Изменение операции на сцене
{
    if(this->items(x,y-height,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty() || operations.at(index)->pos() == QPointF(x,y))
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
    this->removeItem(operations.at(index));                                                             //Удаленяем операцию со сцены
    operations.remove(index);                                                                           //Удаляем указатель на операцию из вектора
    this->update();                                                                                     //Обновляем сцену
}
void MyGraphicScene::drawBackground(QPainter *painter, const QRectF &rect)                              //Отрисовка заднего фона
{
    Q_UNUSED(rect);
    painter->setPen(QPen(Qt::black));                                                                   //Устанавливаем цвет пера
    painter->drawLine(-width/2+59,height/2-15,width,height/2-15);                                       //Рисуем линию на весь размер представления
    for(int i=-width/2+59;i<width/2+30;i=i+30)                                                          //Рисуем вертикальные линии
    {
        painter->drawLine(i,height/2-25,i,height/2-5);
    }
    int x=0;
    for(int i=-width/2+59;i<width/2+60;i=i+60)                                                          //Рисуем значения
    {
        if(i<=width/2+59)
            painter->drawText(i,height/2+5,QString::number(x));
        else
            painter->drawText(i/2+15,height/2+5,QString::number(x));
        x +=60;
    }
    painter->drawLine(width/2+40,height/2-30,width/2+57,height/2-15);                                   //Рисуем верхнюю стороны стрелки
    painter->drawLine(width/2+40,height/2,width/2+57,height/2-15);                                      //Рисуем нижнюю стороны стрелки
}
void MyGraphicScene::createQueue(int coord)
{
    if(listOperations.count() != 0)
    {
        double x = -width/2+coord;
        double y = height/2-30;
        foreach(Operation *ops, listOperations)
        {
            if(x+ops->width <= width/2+coord)
            {
                ops->setPos(x,y);
                x=x+ops->width;
            }
            else
            {
                oldWidth.push_back(width/2+coord);
                emit increaseView();
            }
            ops->inQueue = true;
            ops->setSceneSize(this->width,this->height);
            ops->setCoordStart(coord);
            ops->setToolTip("Начало операции: "+QString::number((width/2 + ops->pos().x())-coord) + "\nКонец операции: " + QString::number((width/2 + ops->pos().x()+ops->width)-coord));
        }
    }
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
void MyGraphicScene::processingRelease()
{
    double x = -width/2+59;
    foreach(Operation *ops, listOperations)
    {
        x = x+ops->width;
    }
    if(!oldWidth.isEmpty() && x < oldWidth.back()-90)
    {
        oldWidth.pop_back();
        emit decreaseView();
    }
}
void MyGraphicScene::addList(int index, QString whereToAdd)
{
    if(!operations.empty())
    {
        int find = listOperations.indexOf(operations.at(index));
        if(find != -1)
        {
            if(whereToAdd == "В начало")
                listOperations.swapItemsAt(find,0);
            if(whereToAdd == "В середину")
                listOperations.swapItemsAt(find,listOperations.count()/2);
            if(whereToAdd == "В конец")
                listOperations.swapItemsAt(find,listOperations.count()-1);
        }
        else
        {
            if(whereToAdd == "В начало")
                listOperations.push_front(operations.at(index));
            if(whereToAdd == "В середину")
                listOperations.insert(listOperations.count()/2,operations.at(index));
            if(whereToAdd == "В конец")
                listOperations.push_back(operations.at(index));
        }
    }
}
void MyGraphicScene::inFile()
{
    if(!operations.empty())
        fileInOut->inputFile(operations);
}
void MyGraphicScene::outFile()
{
    QVector<Geometry> vectorGeometry = fileInOut->outputFile();
    if(!vectorGeometry.empty())
    {
        foreach(Geometry geom, vectorGeometry)
        {
            this->addOperations(geom.x,geom.y,geom.width,geom.height);
        }
    }
}
