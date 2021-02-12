#ifndef MYGRAPHICSCENE_H
#define MYGRAPHICSCENE_H

#include <QGraphicsScene>
#include "operation.h"

class MyGraphicScene:public QGraphicsScene
{
public:
    MyGraphicScene();
    MyGraphicScene(QObject *object = nullptr);
    virtual ~MyGraphicScene() override;
    QString addOperations(double x=0, double y=0, double width=100, double height=30);
    void updateOperations(int index,double x, double y, double width, double height);
    void deleteOperations(int index);
    double width;
    double height;
private:
    Operation *op;
    QVector<Operation*> operations;
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
};

#endif // MYGRAPHICSCENE_H
