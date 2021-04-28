#ifndef OPERATION_H
#define OPERATION_H

#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QObject>

class Operation: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Operation(QString name="Операция", QString reduction="", double width=60, double interval=0, bool dynamic=true);
    QRectF boundingRect() const override;                                                     //Метод который определяет внешние границы элемента в виде прямоугольника
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
                                                                                              //Метод отрисовывающий операции
    void transformOperation(QString name, QString reduction, double width, double interval);  //Метод для изменения операции
    void setSceneSize(double widthView, double heightView);                                   //Метод для передачи размеров сцены
    void setCoordStart(int coord);                                                            //Метод для получения координаты начала операции
    static int getCoef();                                                                     //Метод для получения коэффицента ширины операции
    double width;
    double interval = 0;
    const double height = 40;
    bool dynamic=true;                                                                        //Переменная указывающая статическая ли операция
    QString name;
    QString reduction;
private:
    static int id;
    bool hover=false;                                                                         //Переменная указывающая наведен ли курсор на операцию
    double sceneWidth=0;                                                                      //Ширина сцены
    double sceneHeight=0;                                                                     //Высота сцены
    double start=0;                                                                           //Начало операции
    double end=0;                                                                             //Конец операции
    int coordStart=59;                                                                        //Начало сцены
    static int coef;                                                                          //Коэффицент ширины операции
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;                   //Событие происходящее при наведении на операцию
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;                   //Событие происходящее если курсор вышел за пределы операции
signals:
    void changeOperation();                                                                   //Сигнал возникающий при изменении ширины операции
};

#endif // OPERATION_H
