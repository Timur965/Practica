#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygraphicscene.h"
#include <QGraphicsView>
#include <QMouseEvent>
#include <QRegExp>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    MyGraphicScene *scene;                                                  //Объявляем собственную графическую сцену
    MyGraphicScene *scene2;
    QString nameOperations;
protected:
    virtual bool eventFilter(QObject *object, QEvent *event) override;      // Определяем виртуальный метод для обработки события мыши
private slots:
    void on_pushButton_clicked();                                           //Добавление на сцену
    void on_pushButton_2_clicked();                                         //Изменение на сцену
    void on_pushButton_3_clicked();                                         //Удаление со сцены
};
#endif // MAINWINDOW_H
