#ifndef MYGRAPHICSCENE_H
#define MYGRAPHICSCENE_H

#include <QGraphicsScene>
#include <QList>
#include "operation.h"
#include "fileinout.h"
#include "onoff.h"
#include <QLabel>

class MyGraphicScene:public QGraphicsScene
{
    Q_OBJECT
public:
    MyGraphicScene();
    MyGraphicScene(QObject *object = nullptr);
    virtual ~MyGraphicScene() override;
    bool addOperations(QString name="Operation",double x=0, double y=0, double width=60, double height=30, bool dynamic=true, bool inQueue=false);
                                                                                                                                   // Метод для добавления операции на сцену
    bool updateOperations(int index, QString name, double x, double y, double width, double height);                               // Метод для обновления операции на сцене
    void deleteOperations(int index);                                                                                              // Метод для удаления операции со сцены
    void createQueue(int coord = 60);
    void addList(int index, QString whereToAdd);
    void updateCoordOperations(int coord = 60);
    bool inFile(QString path);
    bool outFile(QString path);
    bool setMinWidthOperation(int index);
    bool setMaxWidthOperation(int index);
    void on(int coord=59);
    void off(int coord=59);
    void recordingInformation();
    QString getCurrentName();
    QVector<QString> getNamesOperations();
    double width;
    double height;
private:
    Operation *op = nullptr;
    FileInOut *fileInOut = nullptr;
    onOff *On=nullptr;
    onOff *Off=nullptr;
    QVector<Operation*> operations;
    QList<Operation*> listOperations;
    QVector<double> oldWidth;
    QVector<Geometry> *vectorGeometry = nullptr;
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
