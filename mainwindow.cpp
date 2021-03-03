#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new MyGraphicScene(this);                                                      //Инициализируем графическую сцену
    scene->setSceneRect(28,10,ui->graphicsView->width()-28,ui->graphicsView->height()-10); //Устанавливаем границы сцены
    ui->graphicsView->setScene(scene);                                                     //Устанавливаем текущую сцену равной scene
    ui->graphicsView->installEventFilter(this);                                            //Устанавливаем фильтр события на представление
    setWindowTitle("Разработка циклограммы");                                              //Устанавливаем заголовок окну
    ui->comboBox->setDuplicatesEnabled(true);
    ui->comboBox_3->addItem("В начало");
    ui->comboBox_3->addItem("В середину");
    ui->comboBox_3->addItem("В конец");
    connect(scene,&MyGraphicScene::increaseView,this,&MainWindow::updateIncreaseView);
    connect(scene,&MyGraphicScene::decreaseView,this,&MainWindow::updateDecreaseView);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(scene != nullptr)
        delete scene;
}

void MainWindow::on_pushButton_clicked()                                                   //Кнопка добавления
{
    QRegExp rgx("\\d{1,3}");                                                               //Создаём регулярное выражение
    if(rgx.indexIn(ui->coordX->text()) != -1 && rgx.indexIn(ui->coordY->text()) != -1 && rgx.indexIn(ui->operationWidth->text()) != -1 && rgx.indexIn(ui->operationHeight->text()) != -1)
    {                                                                                      //Проверка что вводятся цифры
        if(ui->coordX->text().toDouble() > (ui->graphicsView->width()/2 - ui->graphicsView->width())+55 &&
           ui->coordX->text().toDouble() < ui->graphicsView->width()/2 - 40 &&
           ui->coordY->text().toDouble() > (ui->graphicsView->height()/2 - ui->graphicsView->height())+80 &&
           ui->coordY->text().toDouble() < ui->graphicsView->height()/2 - 25 &&
           ui->operationWidth->text().toDouble() >=10 &&
           ui->operationWidth->text().toDouble() < ui->graphicsView->width() &&
           ui->operationHeight->text().toDouble() >=10 &&
           ui->operationHeight->text().toDouble() < ui->graphicsView->height())             //Проверка чтобы вводимые данные не выходили за границы представления
        {
            if(scene->addOperations(ui->coordX->text().toDouble(),ui->coordY->text().toDouble(),ui->operationWidth->text().toDouble(),ui->operationHeight->text().toDouble()))
            {
                ui->comboBox->addItem(scene->getName());
                ui->comboBox_2->addItem(scene->getName());
            }
            else
                QMessageBox::warning(this,"Добавление","Координаты заняты другим объектом.");
        }
        else
        {
             QMessageBox::warning(this,"Неккоректные данные","Вы ввели слишком большие или\n слишко малые координаты.");
        }
    }
    else
    {
        QMessageBox::warning(this,"Неккоректные данные","Вводить можно только цифры");
    }
}

void MainWindow::on_pushButton_2_clicked()                                                 //Кнопка изменения
{
    QRegExp rgx("\\d{1,3}");                                                               //Создаём регулярное выражение
    if(rgx.indexIn(ui->newCoordX->text()) != -1 || rgx.indexIn(ui->newCoordY->text()) != -1 || rgx.indexIn(ui->newOperationWidth->text()) != -1 || rgx.indexIn(ui->newOperationHeight->text()) != -1)
    {                                                                                      //Проверка что вводятся цифры
        if(ui->newCoordX->text().toDouble() > (ui->graphicsView->width()/2 - ui->graphicsView->width())+55 &&
           ui->newCoordX->text().toDouble() < ui->graphicsView->width()/2 - 40 &&
           ui->newCoordY->text().toDouble() > (ui->graphicsView->height()/2 - ui->graphicsView->height())+80 &&
           ui->newCoordY->text().toDouble() < ui->graphicsView->height()/2 - 25 &&
           ui->newOperationWidth->text().toDouble() >=10 &&
           ui->newOperationWidth->text().toDouble() < ui->graphicsView->width() &&
           ui->newOperationHeight->text().toDouble() >=10 &&
           ui->newOperationHeight->text().toDouble() < ui->graphicsView->height())          //Проверка чтобы вводимые данные не выходили за границы представления
        {
            if(ui->comboBox->count() != NULL)                                               //Проверка если количество записей в combobox не равно 0
            {
                if(!scene->updateOperations(ui->comboBox->currentIndex(),ui->newCoordX->text().toDouble(),ui->newCoordY->text().toDouble(),ui->newOperationWidth->text().toDouble(),ui->newOperationHeight->text().toDouble()))
                {
                    QMessageBox::warning(this,"Изменение","Координаты заняты другим объектом.");
                }
            }
            else
            {
                 QMessageBox::warning(this,"Неккоректные данные","Выберите операцию для изменения");
            }
        }
        else
        {
            QMessageBox::warning(this,"Неккоректные данные","Вы ввели слишком большие или\n слишко малые координаты.");
        }
    }
    else
    {
        QMessageBox::warning(this,"Неккоректные данные","Вводить можно только цифры");
    }
}
void MainWindow::on_pushButton_3_clicked()                                                 //Кнопка удаления
{
    if(ui->comboBox->count() != NULL)                                                      //Проверка если количество записей в combobox не равно 0
    {
        int index = ui->comboBox->currentIndex();
        ui->comboBox->removeItem(index);
        scene->deleteOperations(index);
    }
    else
    {
        QMessageBox::warning(this,"Неккоректные данные","Выберите операцию для удаления");
    }
}
bool MainWindow::eventFilter(QObject *object, QEvent *event)                                //Виртуальный метод для обработки события мыши
{
    scene->width = ui->graphicsView->width();
    scene->height = ui->graphicsView->height();
    if(event->type() == QEvent::MouseButtonDblClick)                                        //Если тип события равен двойному нажатию кнопки мыши
    {
        if(object == ui->graphicsView)                                                      //Если объект равен представлению
        {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);                            //Преобразуем event в QMouseevent
            if(me->button() == Qt::LeftButton)                                              //Если кнопка равна  левой кнопке мыши
            {
                if(scene->addOperations())
                {
                    ui->comboBox->addItem(scene->getName());
                    ui->comboBox_2->addItem(scene->getName());
                }
                else
                    QMessageBox::warning(this,"Добавление","Координаты заняты другим объектом.");
            }
        }
    }
    return QObject::eventFilter(object,event);
}
void MainWindow::resizeEvent(QResizeEvent *)
{
    scene->createQueue();
}

void MainWindow::on_pushButton_4_clicked()
{
    scene->addList(ui->comboBox_2->currentIndex(),ui->comboBox_3->currentText());
    scene->createQueue();
}
void MainWindow::updateIncreaseView()
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() + 240);
    scene->createQueue();
}
void MainWindow::updateDecreaseView()
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() - 240);
    ui->graphicsView->setMaximumWidth(ui->graphicsView->width() + 240);
    scene->createQueue(179);
}

void MainWindow::on_pushButton_5_clicked()
{
    scene->inFile();
}

void MainWindow::on_pushButton_6_clicked()
{
    scene->outFile();
    ui->pushButton_6->setEnabled(false);
}
