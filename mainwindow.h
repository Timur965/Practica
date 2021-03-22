#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygraphicscene.h"
#include "database.h"
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
    void mode(QString name);
    void viewEquipment(int id);
private:
    Ui::MainWindow *ui;
    MyGraphicScene *scene=nullptr;                                          //Объявляем собственную графическую сцену
    DataBase *db=nullptr;
    int idEquipment;
    bool clickOn=false;
    bool clickOff=false;
    bool isMode=false;
protected:
    virtual bool eventFilter(QObject *object, QEvent *event) override;      // Определяем виртуальный метод для обработки события мыши
    virtual void resizeEvent(QResizeEvent *) override;
private slots:
    void updateIncreaseView();
    void updateDecreaseView();
    void on_comboBox_4_currentIndexChanged(int index);
    void on_setMinWidthOperation_clicked();
    void on_setMaxWidthOperation_clicked();
    void on_InputDB_clicked();
    void on_OutputDB_clicked();
    void on_OnOffcyclogram_clicked();
    void on_AddOperation_clicked();
    void on_UpdateOperation_clicked();
    void on_DeleteOperation_clicked();
    void on_AddQueue_clicked();
    void on_InputFile_clicked();
    void on_OutputFile_clicked();
};
#endif // MAINWINDOW_H
