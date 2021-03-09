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
    ui->graphicsView->setMinimumWidth(0);
    ui->graphicsView->setMaximumWidth(754);
    ui->graphicsView->installEventFilter(this);                                            //Устанавливаем фильтр события на представление
    setWindowTitle("Разработка циклограммы");                                              //Устанавливаем заголовок окну
    ui->comboBox->setDuplicatesEnabled(true);
    ui->comboBox_3->addItem("В начало");
    ui->comboBox_3->addItem("В середину");
    ui->comboBox_3->addItem("В конец");
    connect(scene,&MyGraphicScene::increaseView,this,&MainWindow::updateIncreaseView);
    connect(scene,&MyGraphicScene::decreaseView,this,&MainWindow::updateDecreaseView);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->graphicsView->setEnabled(false);
    ui->comboBox_4->addItem("Динамический");
    ui->comboBox_4->addItem("Статический");
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
    bool dynamic=true;
    if(rgx.indexIn(ui->coordX->text()) != -1 && rgx.indexIn(ui->coordY->text()) != -1 && rgx.indexIn(ui->operationWidth->text()) != -1 && rgx.indexIn(ui->operationHeight->text()) != -1)
    {                                                                                      //Проверка что вводятся цифры
        rgx.setPattern("^[A-Z]|[a-z]\\d{1,3}$");
        if(rgx.indexIn(ui->nameOperations->text()) != -1)
        {
            if(ui->coordX->text().toDouble() > (ui->graphicsView->width()/2 - ui->graphicsView->width())+55 &&
               ui->coordX->text().toDouble() < ui->graphicsView->width()/2 - 40 &&
               ui->coordY->text().toDouble() > (ui->graphicsView->height()/2 - ui->graphicsView->height())+80 &&
               ui->coordY->text().toDouble() < ui->graphicsView->height()/2 - 25 &&
               ui->operationWidth->text().toDouble() >=1 &&
               ui->operationWidth->text().toDouble() <= 70 &&
               ui->operationHeight->text().toDouble() >=30 &&
               ui->operationHeight->text().toDouble() < 150)             //Проверка чтобы вводимые данные не выходили за границы представления
            {
                if(ui->comboBox_4->currentIndex() == 0)
                {
                    dynamic = true;
                }
                else
                {
                    dynamic = false;
                }
                if(scene->addOperations(ui->nameOperations->text(),ui->coordX->text().toDouble(),ui->coordY->text().toDouble(),ui->operationWidth->text().toDouble(),ui->operationHeight->text().toDouble(),dynamic))
                {
                    ui->comboBox->addItem(scene->getCurrentName());
                    ui->comboBox_2->addItem(scene->getCurrentName());
                    ui->comboBox_5->addItem(scene->getCurrentName());
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
            QMessageBox::warning(this,"Неккоректные данные","Введите правильное название операции");
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
        rgx.setPattern("^[A-Z]|[a-z]\\d{1,3}$");
        if(rgx.indexIn(ui->newNameOperations->text()) != -1)
        {
            if(ui->newCoordX->text().toDouble() > (ui->graphicsView->width()/2 - ui->graphicsView->width())+55 &&
               ui->newCoordX->text().toDouble() < ui->graphicsView->width()/2 - 40 &&
               ui->newCoordY->text().toDouble() > (ui->graphicsView->height()/2 - ui->graphicsView->height())+80 &&
               ui->newCoordY->text().toDouble() < ui->graphicsView->height()/2 - 25 &&
               ui->newOperationWidth->text().toDouble() >=1 &&
               ui->newOperationWidth->text().toDouble() <= 70 &&
               ui->newOperationHeight->text().toDouble() >=30 &&
               ui->newOperationHeight->text().toDouble() <= 150)                            //Проверка чтобы вводимые данные не выходили за границы представления
            {
                if(ui->comboBox->count() != NULL)                                           //Проверка если количество записей в combobox не равно 0
                {
                    if(!scene->updateOperations(ui->comboBox->currentIndex(),ui->newNameOperations->text(),ui->newCoordX->text().toDouble(),ui->newCoordY->text().toDouble(),ui->newOperationWidth->text().toDouble(),ui->newOperationHeight->text().toDouble()))
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
            QMessageBox::warning(this,"Неккоректные данные","Введите правильное название операции");
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
        ui->comboBox_2->removeItem(index);
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
    if(event->type() == QEvent::MouseButtonDblClick && ui->graphicsView->isEnabled())       //Если тип события равен двойному нажатию кнопки мыши
    {
        bool dynamic=true;
        if(ui->comboBox_4->currentIndex() == 0)
        {
            dynamic = true;
        }
        else
        {
            dynamic = false;
        }
        if(object == ui->graphicsView)                                                      //Если объект равен представлению
        {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);                            //Преобразуем event в QMouseevent
            if(me->button() == Qt::LeftButton)                                              //Если кнопка равна  левой кнопке мыши
            {
                if(scene->addOperations("",0,0,60,30,dynamic))
                {
                    ui->comboBox->addItem(scene->getCurrentName());
                    ui->comboBox_2->addItem(scene->getCurrentName());
                    ui->comboBox_5->addItem(scene->getCurrentName());
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
    if(ui->graphicsView->isEnabled())
    {
        scene->on();
    }
    if(!ui->graphicsView->isEnabled() && clickOff)
    {
        scene->on();
        scene->off();
    }
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
    scene->updateCoordOperations();
    scene->on();
}
void MainWindow::updateDecreaseView()
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() - 240);
    ui->graphicsView->setMaximumWidth(ui->graphicsView->width() + 240);
    scene->createQueue(180);
    scene->updateCoordOperations(180);
    scene->on(179);
}

void MainWindow::on_pushButton_5_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Выберите файл","","*.json");
    if(!path.isEmpty())
    {
        if(!scene->inFile(path))
            QMessageBox::warning(this,"Ошибка с файлом","Данный файл невозможно открыть.");
    }
    else
        QMessageBox::warning(this,"Путь к файлу","Путь к файлу указан неверно.");
}

void MainWindow::on_pushButton_6_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Выберите файл","","*.json");
    if(!path.isEmpty())
    {
        if(!scene->outFile(path))
            QMessageBox::warning(this,"Ошибка с файлом","Данный файл невозможно открыть.");
        else
        {
            ui->comboBox->clear();
            ui->comboBox_2->clear();
            foreach(QString name, scene->getNamesOperations())
            {
                ui->comboBox->addItem(name);
                ui->comboBox_2->addItem(name);
                ui->comboBox_5->addItem(name);
            }
        }
    }
    else
        QMessageBox::warning(this,"Путь к файлу","Путь к файлу указан неверно.");
}

void MainWindow::on_pushButton_7_clicked()
{
    if(!clickOn)
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->graphicsView->setEnabled(true);
        scene->on();
        clickOn = true;
        ui->pushButton_7->setText("Выкл");
    }
    else
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->graphicsView->setEnabled(false);
        ui->setMinWidthOperation->hide();
        ui->setMaxWidthOperation->hide();
        ui->comboBox_5->hide();
        ui->comboBox_5->hide();
        scene->off();
        clickOn = false;
        clickOff = true;
        ui->pushButton_7->setText("Вкл");
    }
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    if(index == 1)
    {
        if(clickOn)
        {
            ui->setMinWidthOperation->show();
            ui->setMaxWidthOperation->show();
            ui->label_15->show();
            ui->comboBox_5->show();
        }
    }
    else
    {
        ui->setMinWidthOperation->hide();
        ui->setMaxWidthOperation->hide();
        ui->comboBox_5->hide();
        ui->label_15->hide();
    }
}

void MainWindow::on_setMinWidthOperation_clicked()
{
    if(ui->comboBox_5->currentIndex() != -1)
    {
        if(!scene->setMinWidthOperation(ui->comboBox_5->currentIndex()))
            QMessageBox::warning(this,"Неккоректная операция","Операция не статическая");
    }
}

void MainWindow::on_setMaxWidthOperation_clicked()
{
    if(ui->comboBox_5->currentIndex() != -1)
    {
        if(!scene->setMaxWidthOperation(ui->comboBox_5->currentIndex()))
            QMessageBox::warning(this,"Неккоректная операция","Операция не статическая");
    }
}
