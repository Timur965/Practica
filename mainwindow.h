#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygraphicscene.h"
#include <QGraphicsView>
#include <QMouseEvent>
#include <QRegExp>
#include <QMessageBox>
#include <QLabel>
#include <QFileDialog>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mode(QString name);
private:
    Ui::MainWindow *ui;
    MyGraphicScene *scene=nullptr;                                          //Объявляем собственную графическую сцену
    bool clickOn=false;
    bool clickOff=false;
protected:
    virtual bool eventFilter(QObject *object, QEvent *event) override;      // Определяем виртуальный метод для обработки события мыши
    virtual void resizeEvent(QResizeEvent *) override;
private slots:
    void on_pushButton_clicked();                                           //Добавление на сцену
    void on_pushButton_2_clicked();                                         //Изменение на сцену
    void on_pushButton_3_clicked();                                         //Удаление со сцены
    void on_pushButton_4_clicked();
    void updateIncreaseView();
    void updateDecreaseView();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_comboBox_4_currentIndexChanged(int index);
    void on_setMinWidthOperation_clicked();
    void on_setMaxWidthOperation_clicked();
};
#endif // MAINWINDOW_H
