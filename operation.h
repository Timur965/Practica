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
    QRectF boundingRect() const override;                                       //Метод который определяет внешние границы элемента в виде прямоугольника
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
                                                                                //Метод отрисовывающий операции
    void transformOperation(QString name, double x, double y, double width, double height);   //Метод для изменения операции
    void setSceneSize(double widthView, double heightView);
    void setCoordStart(int coord);
    bool setMinWidth();
    bool setMaxWidth();
    static int getCoef();
    double x;
    double y;
    double width;
    double height;
    bool dynamic=true;
    QString name;
    bool inQueue;
private:
    static int id;
    bool press=false;
    bool move=false;
    bool hover=false;
    double prx;
    double pry;
    double sceneWidth=0;
    double sceneHeight=0;
    double start=0;
    double end=0;
    int coordStart=59;
    static int coef;
    ResizeOperation *ro;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;     //Событие происходящее при нажатии на операцию
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;      //Событие происходящее при передвежении операции
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;   //Событие происходящее при отпускание операции
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
signals:
    void changeOperation();
    void releaseResizeOperation();
};

#endif // OPERATION_H
