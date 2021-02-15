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
    bool addOperations(double x=0, double y=0, double width=100, double height=30); // Метод для добавления операции на сцену
    bool updateOperations(int index,double x, double y, double width, double height);  // Метод для обновления операции на сцене
    void deleteOperations(int index);                                                  // Метод для удаления операции со сцены
    QString getName();
    double width;
    double height;
private:
    Operation *op;
    QVector<Operation*> operations;
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;        //Метод для отрисовки заднего фона
};

#endif // MYGRAPHICSCENE_H
