#ifndef OPERATION_H
#define OPERATION_H

#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsView>
#include "resizeoperation.h"
#include <QMouseEvent>
#include <QObject>

class Operation: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Operation(QString name="Operation", double x=0, double y=0, double width=60, double height=30, bool dynamic=true, bool inQueue=false);
    QRectF boundingRect() const override;                                                     //Метод который определяет внешние границы элемента в виде прямоугольника
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
                                                                                              //Метод отрисовывающий операции
    void transformOperation(QString name, double x, double y, double width, double height);   //Метод для изменения операции
    void setSceneSize(double widthView, double heightView);                                   //Метод для передачи размеров сцены
    void setCoordStart(int coord);                                                            //Метод для получения координаты начала операции
    bool setMinWidth();                                                                       //Метод для установки минимальной длины операции
    bool setMaxWidth();                                                                       //Метод для установки максимальной длины операции
    static int getCoef();                                                                     //Метод для получения коэффицента ширины операции
    double x;
    double y;
    double width;
    double height;
    bool dynamic=true;                                                                        //Переменная указывающая статическая ли операция
    QString name;
    bool inQueue;                                                                             //Переменная указывающая находится ли операция в очереди
private:
    static int id;
    bool press=false;                                                                         //Переменная указывающая нажаты ли операция
    bool move=false;                                                                          //Переменная указывающая перемещается ли операция
    bool hover=false;                                                                         //Переменная указывающая наведен ли курсор на операцию
    double prx;                                                                               //Предыдущее координата по х
    double pry;                                                                               //Предыдущее координата по у
    double sceneWidth=0;                                                                      //Ширина сцены
    double sceneHeight=0;                                                                     //Высота сцены
    double start=0;                                                                           //Начало операции
    double end=0;                                                                             //Конец операции
    int coordStart=59;                                                                        //Начало сцены
    static int coef;                                                                          //Коэффицент ширины операции
    ResizeOperation *ro;                                                                      //Объект для увеличения ширины операции
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;                   //Событие происходящее при нажатии на операцию
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;                    //Событие происходящее при передвежении операции
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;                 //Событие происходящее при отпускание операции
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;            //Фильтруем события для элемента watched. event это фильтруемое событие
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;                   //Событие происходящее при наведении на операцию
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;                   //Событие происходящее если курсор вышел за пределы операции
signals:
    void changeOperation();                                                                   //Сигнал возникающий при изменении ширины операции
    void releaseResizeOperation();                                                            //Сигнал возникающий после изменения ширины операции
};

#endif // OPERATION_H
