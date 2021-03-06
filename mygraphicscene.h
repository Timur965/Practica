#ifndef MYGRAPHICSCENE_H
#define MYGRAPHICSCENE_H

#include <QGraphicsScene>
#include <QList>
#include "operation.h"
#include <QLabel>

class MyGraphicScene:public QGraphicsScene
{
    Q_OBJECT
public:
    MyGraphicScene();
    MyGraphicScene(QObject *object = nullptr);
    virtual ~MyGraphicScene() override;
    bool addOperations(QString name="Операция", QString reduction="", double width=60, double interval=0, bool dynamic=true);      //Метод для добавления операции на сцену
    bool updateOperations(int index, QString name, QString reduction, double width, double interval);                              //Метод для обновления операции на сцене
    bool deleteOperations(int index);                                                                                              //Метод для удаления операции со сцены
    void createQueue(int coord = 59, int coordHeight = 90);                                                                        //Метод для построения очереди
    void updateCoordOperations(int coord = 60);                                                                                    //Метод для обовления координат операций не добавленных в очередь
    void processingRelease();                                                                                                      //Метод для обработки операции после изменения ширины
    QString getCurrentName();                                                                                                      //Метод для получения названия операции
    QVector<QString> getNamesOperations();                                                                                         //Метод для получения всех названий операций
    QVector<Operation*> getOperations();                                                                                           //Метод для получения всех операций
    void allClear();
    double width;
    double height;
private:
    Operation *op = nullptr;                                                                                                       //Операция
    QVector<Operation*> operations;                                                                                                //Все операции на сцене
    QVector<double> oldWidth;                                                                                                      //Предыдущии размеры сцены
    bool checkName(QString name);
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &) override;                                                       //Метод для отрисовки заднего фона
public slots:
    void processingChange();                                                                                                       //Метод для обработки  операции при изменении ширины
signals:
    void increaseView();                                                                                                           //Сигнал для увелечения ширины представления
    void decreaseView();                                                                                                           //Сигнал для уменьшения ширины представления
    void setSizeGraphicsView();
};

#endif // MYGRAPHICSCENE_H
