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
        delete op;                                                                                                            //Освобождение памяти из под op
    operations.clear();
    qDeleteAll(operations);                                                                                                   //Освобождение памяти из под operations
    listOperations.clear();
    qDeleteAll(listOperations);                                                                                               //Освобождение памяти из под listOperations
}
QString MyGraphicScene::getCurrentName()                                                                                      //Метод для получения названия операции
{
    return op->name;
}
QVector<QString> MyGraphicScene::getNamesOperations()                                                                         //Метод для получения всех названий операций
{
    QVector<QString> temporaryVector;
    foreach(Operation* ops,operations)
    {
        temporaryVector.push_back(ops->name);
    }
    return temporaryVector;
}
QVector<Operation*> MyGraphicScene::getOperations()                                                                          //Метод для получения всех операций
{
    return operations;
}
QList<Operation*> MyGraphicScene::getListOperations()                                                                          //Метод для получения всех операций
{
    return listOperations;
}
bool MyGraphicScene::addOperations(QString name, double x, double y, double width, double interval, bool dynamic, bool inQueue)//Добавление операции на сцену
{
    if(this->items(x,y-40,width,40,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty() || inQueue)    //Проверяем не сталкивается ли новый объект со старыми
    {
        op = new Operation(name,x,y,width,interval,dynamic,inQueue);                                                           //Инициализируем новую операцию
        connect(op,&Operation::changeOperation,this,&MyGraphicScene::processingChange);
        connect(op,&Operation::releaseResizeOperation,this,&MyGraphicScene::processingRelease);
        operations.push_back(op);                                                                                            //Добавляем указатель на операцию в вектор
        if(op->inQueue)
        {
            listOperations.push_back(op);
            this->createQueue(59,60);
        }
        op->setSceneSize(this->width,this->height);
        this->addItem(op);                                                                                                   //Добавляем операцию на сцену
        this->update();                                                                                                      //Обновляем сцену
        return true;
    }
    return false;
}
bool MyGraphicScene::updateOperations(int index, QString name, double x, double y, double width, double interval)
{                                                                                                                            //Изменение операции на сцене
    if(this->items(x,y-40,width,40,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty() || operations.at(index)->pos() == QPointF(x,y))
    {                                                                                                                        //Проверяем не сталкивается ли новый объект со старыми
        operations.at(index)->transformOperation(name,x,y,width,interval);                                                   //Изменяем объект на сцене
        connect(operations.at(index),&Operation::changeOperation,this,&MyGraphicScene::processingChange);
        this->createQueue();
        this->processingRelease();
        this->update();                                                                                                      //Обновляем сцену
        return true;
    }
    return false;
}
void MyGraphicScene::deleteOperations(int index)                                                                             //Удаление операции со сцены
{
    this->removeItem(operations.at(index));                                                                                  //Удаленяем операцию со сцены
    listOperations.removeOne(operations.at(index));
    operations.remove(index);                                                                                                //Удаляем указатель на операцию из вектора
    this->createQueue();
    this->processingRelease();
    this->update();                                                                                                          //Обновляем сцену
}
void MyGraphicScene::drawBackground(QPainter *painter, const QRectF &)                                                       //Отрисовка заднего фона
{
    painter->setPen(QPen(Qt::black));                                                                                        //Устанавливаем цвет пера
    painter->drawLine(-width/2+59,height/2-35,width,height/2-35);                                                            //Рисуем линию на весь размер представления
    for(int i=-width/2+59;i<width/2+30;i=i+30)                                                                               //Рисуем вертикальные линии
    {
        painter->drawLine(i,height/2-45,i,height/2-25);
    }
    int x=0;
    for(int i=-width/2+59;i<(width/2+60);i=i+60)                                                                             //Рисуем значения
    {
        if(i<=width/2+59)
            painter->drawText(i,height/2-15,QString::number(x));
        else
            painter->drawText(i/2+15,height/2-15,QString::number(x));
        x +=60/Operation::getCoef();
    }
    painter->drawLine(width/2+40,height/2-50,width/2+57,height/2-35);                                                        //Рисуем верхнюю стороны стрелки
    painter->drawLine(width/2+40,height/2-20,width/2+57,height/2-35);                                                        //Рисуем нижнюю стороны стрелки
}
void MyGraphicScene::updateCoordOperations(int coord)                                                                        //Метод для обовления координат операций не добавленных в очередь
{
    if(operations.count() != 0)
    {
        foreach(Operation *ops, operations)
        {
            ops->setSceneSize(this->width,this->height);                                                                     //Передаём новые размеры сцены
            ops->setCoordStart(coord);                                                                                       //Передаём новое начало сцены
            this->update();
        }
    }
}
void MyGraphicScene::createQueue(int coord, int coordHeight)                                                                 //Метод для построения очереди
{
    if(listOperations.count() != 0)
    {
        double x = -width/2+coord;                                                                                           //Начало координат по х
        double y = height/2-coordHeight;                                                                                     //Начало координат по у
        foreach(Operation *ops, listOperations)
        {
            if(x+ops->width <= width/2+coord)                                                                                //Если операция не вышла за границу сцены
            {
                ops->setPos(x,y);
                x=x+ops->width + ops->interval;
            }
            else
            {
                oldWidth.push_back(width/2+coord);                                                                           //Записываем текущую ширину сцены
                emit increaseView();                                                                                         //Вызываем сигнал
            }
            ops->inQueue = true;
            ops->setSceneSize(this->width,this->height);                                                                     //Передаём новые размеры сцены
            ops->setCoordStart(coord);                                                                                       //Передаём новое начало сцены
            this->updateCoordOperations(coord);
            this->update();
        }
    }
}
void MyGraphicScene::processingChange()                                                                                      //Слот для обработки  операции при изменении ширины
{
    Operation *focusOperation;
    focusOperation = static_cast<Operation*>(this->focusItem());                                                             //Преобразуем QGraphicsItem* к Operation*
    foreach(Operation *ops, operations)
    {
        if(focusOperation == ops && ops->inQueue)
        {
                this->createQueue();
                break;
        }
    }
}
void MyGraphicScene::processingRelease()                                                                                     //Слот для обработки  операции после изменения ширины
{
    double x = -width/2+59;                                                                                                  //Начало координат по х
    foreach(Operation *ops, listOperations)
    {
        x = x+ops->width + ops->interval;
    }
    if((x < 925/2+59 || listOperations.empty()) && oldWidth.empty())
    {
        emit setSizeGraphicsView();
    }
    else
    {
        for(int i=oldWidth.count()-1; i>=0; i--)
        {
            if(!oldWidth.empty() && x < oldWidth.at(i)-120)                                                                  //Если oldWidth не пустой и ширина всех операций меньше предыдущей ширины
            {
                oldWidth.pop_back();
                emit decreaseView();
            }
        }
    }
    this->createQueue();
}
void MyGraphicScene::addList(int index, QString whereToAdd)                                                                  //Метод для добавления операций в очередь
{
    if(!operations.empty())
    {
        int find = listOperations.indexOf(operations.at(index));                                                             //Получаем индекс операции добавляемой в очередь
        if(find != -1)                                                                                                       //Если этот элемент добавлен в очередь
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
        this->createQueue();
    }
}
void MyGraphicScene::clearList()
{
    listOperations.clear();
}
void MyGraphicScene::recordingInformation()                                                                                  //Метод для отрисовки плана "Запись информации"
{
    addOperations("Т0",0,0,60,5,false,true);                                                                                 //Добавляем операцию
    addOperations("Рк1",0,0,60,10,false,true);                                                                               //Добавляем операцию
    addOperations("Рк2",0,0,10,7,false,true);                                                                                //Добавляем операцию
    addOperations("Рк3",0,0,30,3,false,true);                                                                                //Добавляем операцию
    addOperations("КодМВ",0,0,30,2,false,true);                                                                              //Добавляем операцию
    addOperations("КПИ",0,0,30,0,false,true);                                                                                //Добавляем операцию
    addOperations("Запись информации",0,0,40,0,true,true);                                                                   //Добавляем операцию
    addOperations("Тк",0,0,60,0,false,true);                                                                                 //Добавляем операцию
}
