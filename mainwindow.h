#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygraphicscene.h"
#include "database.h"
#include "fileinout.h"
#include <QGraphicsView>
#include <QMouseEvent>
#include <QRegExp>
#include <QMessageBox>
#include <QLabel>
#include <QFileDialog>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mode(QString name);                                                //Метод для обработки выбранного режима аппаратуры
    void onTime(QString dateTime);                                          //Метод для получения вида аппаратуры
private:
    Ui::MainWindow *ui;
    MyGraphicScene *scene=nullptr;                                          //Объявляем собственную графическую сцену
    DataBase *db=nullptr;
    FileInOut *fileInOut = nullptr;                                         //Указатель класса fileInOut
    QString dateTime;
    QString nameMode;
    void addOperationsInDB(QString nameDatabase);                           //Добавление в таблицу БД
    QVector<Geometry> *vectorGeometry = nullptr;                            //Данные операций из файла
protected:
    virtual bool eventFilter(QObject *object, QEvent *event) override;      //Определяем виртуальный метод для обработки события мыши
    virtual void resizeEvent(QResizeEvent *) override;                      //Определяем виртуальный метод для обработки события изменение окна
private slots:
    void updateIncreaseView();                                              //Слот для увелечения представления
    void updateDecreaseView();                                              //Слот для уменьшения представления
    void updateSizeView();
    void on_InputDB_clicked();                                              //Слот для записи данных в БД
    void on_OutputDB_clicked();                                             //Слот для считывания данных из БД
    void on_AddOperation_clicked();                                         //Слот для добавления операции на сцену
    void on_UpdateOperation_clicked();                                      //Слот для обновлении операции на сцене
    void on_DeleteOperation_clicked();                                      //Слот для удаления операции со сцены
    void on_AddQueue_clicked();                                             //Слот для добавления операции в очередь
    void on_InputFile_clicked();                                            //Слот для записи данных в файл
    void on_OutputFile_clicked();                                           //Слот для считывания данных из файла
    void on_connectDB_clicked();                                            //Слот для установки и разрыва соединения с БД
    void on_comboBox_4_currentIndexChanged(int index);
};
#endif // MAINWINDOW_H
