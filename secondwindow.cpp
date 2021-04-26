#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
    setWindowTitle("Выбор режима");                                //Установили заголовок окна
    timer = new QTimer();
    timer->setInterval(1000);
    timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
    ui->mode->addItem("Запись информации");
    ui->mode->addItem("Воспроизведение информации");
}

SecondWindow::~SecondWindow()
{
    delete ui;
    if(w != nullptr)
        delete w;                                                               //Освобождение памяти из под w
    if(timer != nullptr)
        delete timer;
}

void SecondWindow::on_pushButton_clicked()                                      //Слот для открытия нового окна
{
    timer->stop();
    w = new MainWindow();
    w->show();
    w->onTime(ui->label->text());
    w->mode(ui->mode->currentText());
    this->close();
}
void SecondWindow::timerTimeout()
{
    ui->label->setText("Текущее время: "+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}
