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
    if(On != nullptr)
        delete On;
    if(Off != nullptr)
        delete Off;
    if(vectorGeometry != nullptr)
        delete vectorGeometry;
}
QString MyGraphicScene::getCurrentName()
{
    return op->name;
}
QVector<QString> MyGraphicScene::getNamesOperations()
{
    QVector<QString> temporaryVector;
    foreach(Operation* ops,operations)
    {
        temporaryVector.push_back(ops->name);
    }
    return temporaryVector;
}
bool MyGraphicScene::addOperations(QString name, double x, double y, double width, double height, bool dynamic, bool inQueue)//Добавление операции на сцену
{
    if(this->items(x,y-height,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty())               //Проверяем не сталкивается ли новый объект со старыми
    {
        op = new Operation(name,x,y,width,height,dynamic,inQueue);                                                           //Инициализируем новую операцию
        connect(op,&Operation::changeOperation,this,&MyGraphicScene::processingChange);
        connect(op,&Operation::releaseResizeOperation,this,&MyGraphicScene::processingRelease);
        operations.push_back(op);                                                                                            //Добавляем указатель на операцию в вектор
        if(op->inQueue)
        {
            listOperations.push_back(op);
            this->createQueue();
        }
        op->setSceneSize(this->width,this->height);
        this->addItem(op);                                                                                                   //Добавляем операцию на сцену
        this->update();                                                                                                      //Обновляем сцену
        return true;
    }
    return false;
}
bool MyGraphicScene::updateOperations(int index, QString name, double x, double y, double width, double height)
{                                                                                                                            //Изменение операции на сцене
    if(this->items(x,y-height,width,height,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform()).isEmpty() || operations.at(index)->pos() == QPointF(x,y))
    {                                                                                                                        //Проверяем не сталкивается ли новый объект со старыми
        operations.at(index)->transformOperation(name,x,y,width,height);                                                     //Изменяем объект на сцене
        connect(operations.at(index),&Operation::changeOperation,this,&MyGraphicScene::processingChange);
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
void MyGraphicScene::drawBackground(QPainter *painter, const QRectF &rect)                                                   //Отрисовка заднего фона
{
    Q_UNUSED(rect);
    painter->setPen(QPen(Qt::black));                                                                                        //Устанавливаем цвет пера
    painter->drawLine(-width/2+59,height/2-35,width,height/2-35);                                                            //Рисуем линию на весь размер представления
    for(int i=-width/2+59;i<width/2+30;i=i+30)                                                                               //Рисуем вертикальные линии
    {
        painter->drawLine(i,height/2-45,i,height/2-25);
    }
    int x=0;
    for(int i=-width/2+59;i<(width/2+60);i=i+60)                                                                               //Рисуем значения
    {
        if(i<=width/2+59)
            painter->drawText(i,height/2-15,QString::number(x));
        else
            painter->drawText(i/2+15,height/2-15,QString::number(x));
        x +=60/Operation::getCoef();
    }
    painter->drawLine(width/2+40,height/2-50,width/2+57,height/2-35);                                                        //Рисуем верхнюю стороны стрелки
    painter->drawLine(width/2+40,height/2-20,width/2+57,height/2-35);                                                           //Рисуем нижнюю стороны стрелки
}
void MyGraphicScene::updateCoordOperations(int coord)
{
    if(operations.count() != 0)
    {
        foreach(Operation *ops, operations)
        {
            ops->setSceneSize(this->width,this->height);
            ops->setCoordStart(coord);
            this->update();
        }
    }
}
void MyGraphicScene::createQueue(int coord, int coordHeight)
{
    if(listOperations.count() != 0)
    {
        double x = -width/2+coord;
        double y = height/2-coordHeight;
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
            this->update();
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
                this->createQueue();
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
    if(!oldWidth.isEmpty() && x < oldWidth.back()-120)
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
bool MyGraphicScene::inFile(QString path)
{
    if(!operations.empty() && fileInOut->inputJSONFile(operations,path))
    {
        return true;
    }
    return false;
}
bool MyGraphicScene::outFile(QString path)
{
    vectorGeometry = new QVector<Geometry>();
    fileInOut->outputJSONFile(path,vectorGeometry);
    if(!vectorGeometry->empty())
    {
        foreach(Geometry geom, *vectorGeometry)
        {
            this->addOperations(geom.name,geom.x,geom.y,geom.width,geom.height,geom.dynamic,geom.inQueue);
        }
        listOperations.clear();
        foreach(Operation *ops, operations)
        {
            if(ops->inQueue)
            {
                listOperations.push_back(ops);
            }
        }
        this->createQueue();
        return true;
    }
    return false;
}
void MyGraphicScene::on(int coord, int coordHeight)
{
    if(On !=nullptr)
    {
        this->removeItem(On);
        delete On;
    }
    if(Off !=nullptr)
    {
        this->removeItem(Off);
        delete Off;
        Off=nullptr;
    }
    On = new onOff();
    On->x = -width/2+coord;
    On->y = height/2-coordHeight;
    On->width = 1;
    On->height = 50;
    On->name = "Т0";
    this->addItem(On);
    this->update();
}
void MyGraphicScene::off(int coord, int coordHeight)
{
    Off = new onOff();
    double x = -width/2+coord;
    foreach(Operation *ops, listOperations)
    {
        x = x+ops->width;
    }
    Off->x = x;
    Off->y = height/2-coordHeight;
    Off->width = 1;
    Off->height = 50;
    Off->name = "Тк";
    this->addItem(Off);
    this->update();
}
bool MyGraphicScene::setMinWidthOperation(int index)
{
    if(operations.at(index)->setMinWidth())
    {
        this->update();
        return true;
    }
    return false;
}
bool MyGraphicScene::setMaxWidthOperation(int index)
{
    if(operations.at(index)->setMaxWidth())
    {
        this->update();
        return true;
    }
    return false;
}
void MyGraphicScene::recordingInformation()
{
    on();
    addOperations("",0,0,60,30,true,true);
    addOperations("",0,0,10,30,true,true);
    addOperations("",0,0,30,30,true,true);
    addOperations("",0,0,30,30,true,true);
    addOperations("",0,0,30,30,true,true);
    addOperations("",0,0,40,30,true,true);
    addOperations("",0,0,10,30,true,true);
    off();
}
