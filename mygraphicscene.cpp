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
void MyGraphicScene::allClear()
{
    this->clear();
    oldWidth.clear();
    operations.clear();
    op = nullptr;
}
bool MyGraphicScene::checkName(QString name)
{
    bool oldName=false;
    QString temporaryName = name;
    foreach(Operation *ops, operations)
    {
        if(name.contains(ops->name))
        {
            oldName = true;
            break;
        }
    }
    int count = temporaryName.remove(0,temporaryName.indexOf(QRegExp("\\d"))).toInt();
    if(name.contains("Запись информации") && count <= 10)
    {
        oldName = false;
    }
    return oldName;
}
bool MyGraphicScene::addOperations(QString name, QString reduction, double width, double interval, bool dynamic)             //Добавление операции на сцену
{
    if(!checkName(name))                                                                                                     //Проверяем не занято ли имя
    {
        op = new Operation(name,reduction,width,interval,dynamic);                                                           //Инициализируем новую операцию
        connect(op,&Operation::changeOperation,this,&MyGraphicScene::processingChange);
        operations.push_back(op);                                                                                            //Добавляем указатель на операцию в вектор
        this->createQueue();
        op->setSceneSize(this->width,this->height);
        this->addItem(op);                                                                                                   //Добавляем операцию на сцену
        this->update();                                                                                                      //Обновляем сцену
        return true;
    }
    return false;
}
bool MyGraphicScene::updateOperations(int index, QString name, QString reduction, double width, double interval)             //Изменение операции на сцене
{
    operations.at(index)->transformOperation(name,reduction,width,interval);                                                 //Изменяем объект на сцене
    connect(operations.at(index),&Operation::changeOperation,this,&MyGraphicScene::processingChange);
    this->createQueue();
    this->processingRelease();
    this->update();                                                                                                          //Обновляем сцену
    return true;
}
bool MyGraphicScene::deleteOperations(int index)                                                                             //Удаление операции со сцены
{
    this->removeItem(operations.at(index));                                                                                  //Удаленяем операцию со сцены
    operations.remove(index);                                                                                                //Удаляем указатель на операцию из вектора
    if(operations.empty())
    {
        this->allClear();
    }
    this->createQueue();
    this->processingRelease();
    this->update();                                                                                                          //Обновляем сцену
    return true;
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
    QFont font = painter->font();
    font.setPointSize(9);
    painter->setFont(font);
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
    if(operations.count() != 0)
    {
        double x = -width/2+coord;                                                                                           //Начало координат по х
        double y = height/2-coordHeight;                                                                                     //Начало координат по у
        foreach(Operation *ops, operations)
        {
            if(x + ops->width + ops->interval <= width/2+coord)                                                              //Если операция не вышла за границу сцены
            {
                ops->setPos(x,y);
                if(ops->dynamic)
                    x = x + ops->width + ops->interval;
                else
                    x = x + ops->interval;
            }
            else
            {
                oldWidth.push_back(width/2+coord);                                                                           //Записываем текущую ширину сцены
                emit increaseView();                                                                                         //Вызываем сигнал
            }
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
        if(focusOperation == ops)
        {
                this->createQueue();
                break;
        }
    }
}
void MyGraphicScene::processingRelease()                                                                                     //Слот для обработки  операции после изменения ширины
{
    double x = -width/2+59;                                                                                                  //Начало координат по х
    foreach(Operation *ops, operations)
    {
        if(ops->dynamic)
            x = x + ops->width + ops->interval;
        else
            x = x + ops->interval;
    }
    if((x < 925/2+59 || operations.empty()) && oldWidth.empty())
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
