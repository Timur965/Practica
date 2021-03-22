#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new MyGraphicScene(this);                                                      //Инициализируем графическую сцену
    scene->setSceneRect(28,10,ui->graphicsView->width()-28,ui->graphicsView->height()-10); //Устанавливаем границы сцены

    db = new DataBase();

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

    ui->AddOperation->setEnabled(false);
    ui->UpdateOperation->setEnabled(false);
    ui->DeleteOperation->setEnabled(false);
    ui->AddQueue->setEnabled(false);
    ui->InputFile->setEnabled(false);
    ui->OutputFile->setEnabled(false);
    ui->graphicsView->setEnabled(false);
    ui->InputDB->setEnabled(false);
    ui->OutputDB->setEnabled(false);

    ui->comboBox_4->addItem("Динамический");
    ui->comboBox_4->addItem("Статический");
}

MainWindow::~MainWindow()
{
    delete ui;
    if(scene != nullptr)
        delete scene;
    if(db != nullptr)
        delete db;
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
    if(clickOn)
    {
        scene->on();
    }
    if(clickOff)
    {
        scene->on();
        scene->off();
    }
    if(isMode && !clickOn)
    {
        scene->on();
        scene->off();
    }
}

void MainWindow::updateIncreaseView()
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() + 240);
    scene->createQueue(60,60);
    scene->updateCoordOperations();
    scene->on();
}
void MainWindow::updateDecreaseView()
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() - 240);
    scene->createQueue(180);
    scene->updateCoordOperations(180);
    scene->on(179);
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
void MainWindow::mode(QString name)
{
    if(name == "Запись информации")
    {
        scene->recordingInformation();
        foreach(QString name, scene->getNamesOperations())
        {
            ui->comboBox->addItem(name);
            ui->comboBox_2->addItem(name);
            ui->comboBox_5->addItem(name);
        }
        isMode = true;
    }
}
void MainWindow::viewEquipment(int id)
{
    idEquipment = id;
}
void MainWindow::on_InputDB_clicked()
{
    if(!ui->LoginDB->text().isEmpty())
    {
        if(!ui->PasswordDB->text().isEmpty())
        {
            QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
            QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
            if(!ui->HostDB->text().isEmpty() && ipRegex.indexIn(ui->HostDB->text()) != -1)
            {
                if(!ui->NameDB->text().isEmpty())
                {
                    if(db->connection(ui->LoginDB->text(),ui->PasswordDB->text(),ui->HostDB->text(),ui->NameDB->text()))
                    {
                        if(!scene->getOperations().isEmpty())
                        {
                            QStringList temporary;
                            QString nameOperations="{";
                            QString geometryOperations="{";
                            QString boolFlag ="{";
                            int i=0;
                            temporary.push_back(QString::number(idEquipment));
                            foreach(Operation *ops, scene->getOperations())
                            {
                                if(i != scene->getOperations().count()-1)
                                {
                                    nameOperations += ops->name+",";
                                    geometryOperations += QString::number(ops->pos().x())+","+QString::number(ops->pos().y())+","+QString::number(ops->width/Operation::getCoef())+","+QString::number(ops->height)+",";
                                    boolFlag += QString::number(ops->dynamic)+","+QString::number(ops->inQueue)+",";
                                }
                                else
                                {
                                    nameOperations += ops->name;
                                    geometryOperations += QString::number(ops->pos().x())+","+QString::number(ops->pos().y())+","+QString::number(ops->width/Operation::getCoef())+","+QString::number(ops->height);
                                    boolFlag += QString::number(ops->dynamic)+","+QString::number(ops->inQueue);
                                }
                                i++;
                            }
                            nameOperations+="}";
                            geometryOperations+="}";
                            boolFlag +="}";
                            temporary.push_back(QDateTime::currentDateTime().time().toString());
                            temporary.push_back(nameOperations);
                            temporary.push_back(geometryOperations);
                            temporary.push_back(boolFlag);
                            if(!db->insertTable("Operations1",temporary))
                            {
                                QMessageBox::warning(this,"Ошибка","Запись данных в таблицу не удалась");
                            }
                        }
                        else QMessageBox::warning(this,"Ошибка","Добавьте операцию на сцену");
                    }
                    else QMessageBox::warning(this,"Ошибка","Не удалось записать данные в БД");
                }
                else QMessageBox::warning(this,"Ошибка","Введите название БД");
            }
            else QMessageBox::warning(this,"Ошибка","Введите IP");
        }
        else QMessageBox::warning(this,"Ошибка","Введите пароль");
    }
    else QMessageBox::warning(this,"Ошибка","Введите логин");
}

void MainWindow::on_OutputDB_clicked()
{
    if(!ui->LoginDB->text().isEmpty())
    {
        if(!ui->PasswordDB->text().isEmpty())
        {
            QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
            QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
            if(!ui->HostDB->text().isEmpty() && ipRegex.indexIn(ui->HostDB->text()) != -1)
            {
                if(!ui->NameDB->text().isEmpty())
                {
                    QStringList data;
                    if(db->connection(ui->LoginDB->text(),ui->PasswordDB->text(),ui->HostDB->text(),ui->NameDB->text()))
                    {
                        if(db->outputFromTable("Cyclogram","Operations1",&data))
                        {
                            QStringList line,nameOps,geometryOps,boolFlag;
                            line = data[0].split(",{");
                            nameOps = line.at(1).split(",");
                            nameOps.last().truncate(nameOps.last().count()-1);
                            geometryOps = line.at(2).split(",");
                            geometryOps.last().truncate(geometryOps.last().count()-1);
                            boolFlag = line.at(3).split(",");
                            boolFlag.last().truncate(boolFlag.last().count()-1);
                            foreach(QString str, nameOps)
                            {
                                if(boolFlag.at(0) == "t")
                                {
                                    if(boolFlag.at(1) == "t")
                                        scene->addOperations(str,geometryOps.at(0).toDouble(),geometryOps.at(1).toDouble(),geometryOps.at(2).toDouble(),geometryOps.at(3).toDouble(),true,true);
                                    else
                                        scene->addOperations(str,geometryOps.at(0).toDouble(),geometryOps.at(1).toDouble(),geometryOps.at(2).toDouble(),geometryOps.at(3).toDouble(),true,false);
                                }
                                else
                                {
                                    if(boolFlag.at(1) == "t")
                                        scene->addOperations(str,geometryOps.at(0).toDouble(),geometryOps.at(1).toDouble(),geometryOps.at(2).toDouble(),geometryOps.at(3).toDouble(),false,true);
                                    else
                                        scene->addOperations(str,geometryOps.at(0).toDouble(),geometryOps.at(1).toDouble(),geometryOps.at(2).toDouble(),geometryOps.at(3).toDouble(),false,false);
                                }
                                geometryOps.removeFirst();
                                geometryOps.removeFirst();
                                geometryOps.removeFirst();
                                geometryOps.removeFirst();
                                boolFlag.removeFirst();
                                boolFlag.removeFirst();
                            }
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
                    else QMessageBox::warning(this,"Ошибка","Не удалось считать данные из БД");
                }
                else QMessageBox::warning(this,"Ошибка","Введите название БД");
            }
            else QMessageBox::warning(this,"Ошибка","Введите IP");
        }
        else QMessageBox::warning(this,"Ошибка","Введите пароль");
    }
    else QMessageBox::warning(this,"Ошибка","Введите логин");
}

void MainWindow::on_OnOffcyclogram_clicked()
{
    if(!clickOn)
    {
        ui->AddOperation->setEnabled(true);
        ui->UpdateOperation->setEnabled(true);
        ui->DeleteOperation->setEnabled(true);
        ui->AddQueue->setEnabled(true);
        ui->InputFile->setEnabled(true);
        ui->OutputFile->setEnabled(true);
        ui->graphicsView->setEnabled(true);
        ui->InputDB->setEnabled(true);
        ui->OutputDB->setEnabled(true);
        scene->on();
        clickOn = true;
        ui->OnOffcyclogram->setText("Выкл");
    }
    else
    {
        ui->AddOperation->setEnabled(false);
        ui->UpdateOperation->setEnabled(false);
        ui->DeleteOperation->setEnabled(false);
        ui->AddQueue->setEnabled(false);
        ui->InputFile->setEnabled(false);
        ui->OutputFile->setEnabled(false);
        ui->graphicsView->setEnabled(false);
        ui->InputDB->setEnabled(false);
        ui->OutputDB->setEnabled(false);
        ui->setMinWidthOperation->hide();
        ui->setMaxWidthOperation->hide();
        ui->comboBox_5->hide();
        scene->off();
        clickOn = false;
        clickOff = true;
        ui->OnOffcyclogram->setText("Вкл");
    }
}

void MainWindow::on_AddOperation_clicked()
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
               ui->coordY->text().toDouble() < ui->graphicsView->height()/2 - 45 &&
               ui->operationWidth->text().toDouble() >=1 &&
               ui->operationWidth->text().toDouble() <= 70 &&
               ui->operationHeight->text().toDouble() >= 30 &&
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

void MainWindow::on_UpdateOperation_clicked()
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
               ui->newCoordY->text().toDouble() < ui->graphicsView->height()/2 - 45 &&
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

void MainWindow::on_DeleteOperation_clicked()
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

void MainWindow::on_AddQueue_clicked()
{
    scene->addList(ui->comboBox_2->currentIndex(),ui->comboBox_3->currentText());
    scene->createQueue();
}

void MainWindow::on_InputFile_clicked()
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

void MainWindow::on_OutputFile_clicked()
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
