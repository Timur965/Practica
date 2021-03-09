#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
    setWindowTitle("Выбор режима");
}

SecondWindow::~SecondWindow()
{
    delete ui;
    if(w != nullptr)
        delete w;
}

void SecondWindow::on_pushButton_clicked()
{
    w = new MainWindow();
    if(ui->radioButton->isChecked())
    {
        w->show();
        w->mode(ui->radioButton->text());
        this->close();
    }
}
