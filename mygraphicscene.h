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
                                                                                                                                   //Метод для добавления операции на сцену
    bool updateOperations(int index, QString name, double x, double y, double width, double height);                               //Метод для обновления операции на сцене
    void deleteOperations(int index);                                                                                              //Метод для удаления операции со сцены
    void createQueue(int coord = 60, int coordHeight = 50);                                                                        //Метод для построения очереди
    void addList(int index, QString whereToAdd);                                                                                   //Метод для добавления операций в очередь
    void updateCoordOperations(int coord = 60);                                                                                    //Метод для обовления координат операций не добавленных в очередь
    bool inFile(QString path);                                                                                                     //Метод для записи в файл
    bool outFile(QString path);                                                                                                    //Метод для считывания из файла
    bool setMinWidthOperation(int index);                                                                                          //Метод для установки минимальной ширины операции
    bool setMaxWidthOperation(int index);                                                                                          //Метод для установки максимальной ширины операции
    void on(int coord=59, int coordHeight = 25);                                                                                   //Метод для отрисовки начала циклограммы
    void off(int coord=59,int coordHeight = 25);                                                                                   //Метод для отрисовки конца циклограммы
    void recordingInformation();                                                                                                   //Метод для отрисовки плана "Запись информации"
    QString getCurrentName();                                                                                                      //Метод для получения названия операции
    QVector<QString> getNamesOperations();                                                                                         //Метод для получения всех названий операций
    QVector<Operation*> getOperations();                                                                                           //Метод для получения всех операций
    double width;
    double height;
private:
    Operation *op = nullptr;                                                                                                       //Операция
    FileInOut *fileInOut = nullptr;                                                                                                //Указатель класса fileInOut
    onOff *On=nullptr;                                                                                                             //Начало циклограммы
    onOff *Off=nullptr;                                                                                                            //Начало циклограммы
    QVector<Operation*> operations;                                                                                                //Все операции на сцене
    QList<Operation*> listOperations;                                                                                              //Операции которые в очереди
    QVector<double> oldWidth;                                                                                                      //Предыдущии размеры сцены
    QVector<Geometry> *vectorGeometry = nullptr;                                                                                   //Данные операций из файла
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;                                                   //Метод для отрисовки заднего фона
private slots:
    void processingChange();                                                                                                       //Слот для обработки  операции при изменении ширины
    void processingRelease();                                                                                                      //Слот для обработки операции после изменения ширины
signals:
    void increaseView();                                                                                                           //Сигнал для увелечения ширины представления
    void decreaseView();                                                                                                           //Сигнал для уменьшения ширины представления
};

#endif // MYGRAPHICSCENE_H
