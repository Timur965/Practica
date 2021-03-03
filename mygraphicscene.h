#ifndef MYGRAPHICSCENE_H
#define MYGRAPHICSCENE_H

#include <QGraphicsScene>
#include <QList>
#include "operation.h"
#include "fileinout.h"
#include <QLabel>

class MyGraphicScene:public QGraphicsScene
{
    Q_OBJECT
public:
    MyGraphicScene();
    MyGraphicScene(QObject *object = nullptr);
    virtual ~MyGraphicScene() override;
    bool addOperations(double x=0, double y=0, double width=100, double height=30);    // Метод для добавления операции на сцену
    bool updateOperations(int index,double x, double y, double width, double height);  // Метод для обновления операции на сцене
    void deleteOperations(int index);                                                  // Метод для удаления операции со сцены
    void createQueue(int coord = 59);
    void addList(int index, QString whereToAdd);
    void inFile();
    void outFile();
    QString getName();
    double width;
    double height;
private:
    Operation *op = nullptr;
    QVector<Operation*> operations;
    QList<Operation*> listOperations;
    QVector<double> oldWidth;
    FileInOut *fileInOut = nullptr;
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;        //Метод для отрисовки заднего фона
private slots:
    void processingChange();
    void processingRelease();
signals:
    void increaseView();
    void decreaseView();
};

#endif // MYGRAPHICSCENE_H
