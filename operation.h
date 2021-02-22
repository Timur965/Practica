#ifndef OPERATION_H
#define OPERATION_H

#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsView>
#include <resizeoperation.h>
#include <QMouseEvent>
#include <QObject>

class Operation: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Operation();
    Operation(double x=0, double y=0, double width=100, double height=30);
    QRectF boundingRect() const override;                                       //Метод который определяет внешние границы элемента в виде прямоугольника
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
                                                                                //Метод отрисовывающий операции
    void transformOperation(double x, double y, double width, double height);   //Метод для изменения операции
    double x;
    double y;
    double width;
    double height;
    QString name;
    bool inQueue;
private:
    double prx;
    double pry;
    double sceneWidth;
    double sceneHeight;
    ResizeOperation *ro;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;     //Событие происходящее при нажатии на операцию
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;      //Событие происходящее при передвежении операции
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;   //Событие происходящее при отпускание операции
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;
signals:
    void changeOperation();
};

#endif // OPERATION_H
