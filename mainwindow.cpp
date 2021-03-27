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
    ui->graphicsView->setMaximumWidth(925);
    ui->graphicsView->installEventFilter(this);                                            //Устанавливаем фильтр события на представление

    setWindowTitle("Разработка циклограммы");                                              //Устанавливаем заголовок окну

    ui->comboBox->setDuplicatesEnabled(true);
    ui->comboBox_3->addItem("В начало");
    ui->comboBox_3->addItem("В середину");
    ui->comboBox_3->addItem("В конец");

    connect(scene,&MyGraphicScene::increaseView,this,&MainWindow::updateIncreaseView);     //Соединение сигнала со слотом
    connect(scene,&MyGraphicScene::decreaseView,this,&MainWindow::updateDecreaseView);     //Соединение сигнала со слотом

    ui->AddOperation->setEnabled(false);
    ui->UpdateOperation->setEnabled(false);
    ui->DeleteOperation->setEnabled(false);
    ui->AddQueue->setEnabled(false);
    ui->InputFile->setEnabled(false);
    ui->OutputFile->setEnabled(false);
    ui->graphicsView->setEnabled(false);
    ui->InputDB->setEnabled(false);
    ui->OutputDB->setEnabled(false);
    ui->connectDB->setEnabled(false);

    ui->comboBox_4->addItem("Динамический");
    ui->comboBox_4->addItem("Статический");

    ui->checkout->addItem("Вкл");
    ui->checkout->addItem("Выкл");
    ui->channel1->addItem("Вкл");
    ui->channel1->addItem("Выкл");
    ui->channel2->addItem("Вкл");
    ui->channel2->addItem("Выкл");
}

MainWindow::~MainWindow()
{
    delete ui;
    if(scene != nullptr)
        delete scene;                                                                       //Освобождение памяти из под scene
    if(db != nullptr)
        delete db;                                                                          //Освобождение памяти из под db
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
void MainWindow::resizeEvent(QResizeEvent *)                                                //Определяем виртуальный метод для обработки события изменение окна
{
    scene->createQueue();                                                                   //Создаём очередь
    if(clickOn)                                                                             //Если циклограмма включена
    {
        scene->on();                                                                        //Отрисовываем начало циклограммы
    }
    if(clickOff)                                                                            //Если циклограмма выключена
    {
        scene->on();                                                                        //Отрисовываем начало циклограммы
        scene->off();                                                                       //Отрисовываем конец циклограммы
    }
    if(isMode && !clickOn)                                                                  //Если выбран режим и циклограмма не включена
    {
        scene->on();                                                                        //Отрисовываем начало циклограммы
        scene->off();                                                                       //Отрисовываем конец циклограммы
    }
}

void MainWindow::updateIncreaseView()                                                       //Слот для увелечения представления
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() + 240);                     //Устанавливаем минимальный размер представления
    scene->createQueue(60,60);                                                              //Обновляем координаты операций в очереди
    scene->updateCoordOperations();                                                         //Обновляем координаты операций вне очереди
    scene->on();                                                                            //Отрисовываем начало циклограммы
}
void MainWindow::updateDecreaseView()                                                       //Слот для уменьшения представления
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() - 240);                     //Устанавливаем минимальный размер представления
    scene->createQueue(180);                                                                //Обновляем координаты операций в очереди
    scene->updateCoordOperations(180);                                                      //Обновляем координаты операций вне очереди
    scene->on(179);                                                                         //Отрисовываем начало циклограммы
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)                               //Слот для выбора операции статическая или динамическая
{
    if(index == 1)                                                                          //Если выбрана статическая операция
    {
        if(clickOn)                                                                         //Если циклограмма включена
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

void MainWindow::on_setMinWidthOperation_clicked()                                          //Слот для установки минимальной ширины операции
{
    if(ui->comboBox_5->currentIndex() != -1)
    {
        if(!scene->setMinWidthOperation(ui->comboBox_5->currentIndex()))
            QMessageBox::warning(this,"Неккоректная операция","Операция не статическая");
    }
}

void MainWindow::on_setMaxWidthOperation_clicked()                                          //Слот для установки максимальной ширины операции
{
    if(ui->comboBox_5->currentIndex() != -1)
    {
        if(!scene->setMaxWidthOperation(ui->comboBox_5->currentIndex()))
            QMessageBox::warning(this,"Неккоректная операция","Операция не статическая");
    }
}
void MainWindow::mode(QString name)                                                         //Метод для обработки выбранного режима аппаратуры
{
    if(name == "Запись информации")
    {
        scene->recordingInformation();                                                      //Вызываем метод для отрисовки плана "Запись информации"
        foreach(QString name, scene->getNamesOperations())                                  //Записываем все названия операций в combobox
        {
            ui->comboBox->addItem(name);
            ui->comboBox_2->addItem(name);
            ui->comboBox_5->addItem(name);
        }
        isMode = true;
    }
}
void MainWindow::viewEquipment(int id)                                                      //Метод для получения вида аппаратуры
{
    idEquipment = id;
}
int MainWindow::addMoscowTimeInDB(QString nameDatabase)                                     //Добавление в таблицу БД
{
    QStringList temporary;
    if(!db->outputFromTable(nameDatabase,"MoscowTime",&temporary))                          //Если не удалось считать данные из БД
    {
        QMessageBox::warning(this,"Ошибка при считывании","Не удалось записать данные в таблицу MoscowTime");
        return -1;
    }
    else
    {
        int n = 0;
        for(int i=0; i<temporary.count();i++)
        {
            if(i == temporary.count()-1)
            {
                n = temporary.at(i).split(",").at(0).toInt()+1;                             //Устанавливаем последний id из БД
            }
        }
        temporary.clear();
        temporary.push_back(QString::number(n));
        temporary.push_back(QString::number(QDateTime::currentDateTime().date().year()));
        temporary.push_back(QString::number(QDateTime::currentDateTime().date().month()));
        temporary.push_back(QString::number(QDateTime::currentDateTime().date().day()));
        temporary.push_back(QDateTime::currentDateTime().time().toString());
        if(!db->insertTable("MoscowTime",temporary))                                        //Если не удалось добавить данные в БД
        {
            QMessageBox::warning(this,"Ошибка при записи","Не удалось записать данные в таблицу MoscowTime");
        }
        temporary.clear();
        return n;
    }
}
int MainWindow::addRelayCommandsInDB(QString nameDatabase)                                  //Добавление в таблицу БД
{
    QStringList temporary;
    if(!db->outputFromTable(nameDatabase,"RelayCommands",&temporary))                       //Если не удалось считать данные из БД
    {
        QMessageBox::warning(this,"Ошибка при считывании","Не удалось записать данные в таблицу RelayCommands");
        return -1;
    }
    else
    {
        int n = 0;
        for(int i=0; i<temporary.count();i++)
        {
            if(i == temporary.count()-1)
            {
                n = temporary.at(i).split(",").at(0).toInt()+1;                             //Устанавливаем последний id из БД
            }
        }
        temporary.clear();
        temporary.push_back(QString::number(n));
        if(!clickOff)                                                                       //Если циклограмма не выключена
        {
            if(clickOn)                                                                     //Если циклограмма включена
                temporary.push_back("true");
            else
                temporary.push_back("false");
            temporary.push_back("false");
        }
        else
        {
            temporary.push_back("false");
            temporary.push_back("true");
        }
        if(ui->checkout->currentIndex() == 0)                                               //Если расчековка включена
           temporary.push_back("true");
        else
           temporary.push_back("false");
        temporary.push_back(ui->reserveBits->text());
        if(ui->channel1->currentIndex() == 0)                                               //Если 1 канал включен
           temporary.push_back("true");
        else
           temporary.push_back("false");
        if(ui->channel2->currentIndex() == 0)                                               //Если 2 канал включен
           temporary.push_back("true");
        else
           temporary.push_back("false");
        if(!db->insertTable("RelayCommands",temporary))                                     //Если не удалось добавить данные в БД
        {
            QMessageBox::warning(this,"Ошибка при записи","Не удалось записать данные в таблицу RelayCommands");
        }
        temporary.clear();
        return n;
    }
}
void MainWindow::addEquipmentInDB(int idMoscowTime, int idRelayCommands)                    //Добавление в таблицу БД
{
    QStringList temporary;
    temporary.push_back(QString::number(idEquipment));
    temporary.push_back(QString::number(idMoscowTime));
    temporary.push_back(QString::number(idRelayCommands));
    db->deleteRow("Operations","idEquipment",QString::number(idEquipment),"=");             //Удаляем предыдущии операции данного вида оборудования
    db->deleteRow("Equipment","IdEquipment",QString::number(idEquipment),"=");              //Удаляем предыдущую аппаратуру данного
    if(!db->insertTable("Equipment",temporary))                                             //Если не удалось добавить данные в БД
    {
        QMessageBox::warning(this,"Ошибка при записи","Не удалось записать данные в таблицу Equipment");
    }
    db->deleteRow("MoscowTime","id","(SELECT id FROM \"MoscowTime\", \"Equipment\" WHERE id = \"Equipment\".\"MoscowTime\")","!= all");
                                                                                            //Удаляем не используемое московское время
    db->deleteRow("RelayCommands","id","(SELECT id FROM \"RelayCommands\", \"Equipment\" WHERE id = \"Equipment\".\"RelayCommands\")","!= all");
                                                                                            //Удаляем не используемые релейные команды
    temporary.clear();
}
void MainWindow::addOperationsInDB(QString nameDatabase)                                    //Добавление в таблицу БД
{
    QStringList temporary;
    if(!db->outputFromTable(nameDatabase,"Operations",&temporary))                          //Если не удалось считать данные из БД
    {
        QMessageBox::warning(this,"Ошибка при считывании","Не удалось записать данные в таблицу Operations");
    }
    else
    {
        int n = 0;
        for(int i=0; i<temporary.count();i++)
        {
            if(i == temporary.count()-1)
            {
                n = temporary.at(i).split(",").at(0).toInt()+1;                             //Устанавливаем последний id из БД
            }
        }
        int i=1;
        temporary.clear();
        foreach(Operation *ops, scene->getOperations())
        {
            temporary.push_back(QString::number(n+i));
            temporary.push_back(ops->name);
            temporary.push_back(QString::number(ops->pos().x()));
            temporary.push_back(QString::number(ops->pos().y()));
            temporary.push_back(QString::number(ops->width/Operation::getCoef()));
            temporary.push_back(QString::number(ops->height));
            temporary.push_back(QString::number(ops->dynamic));
            temporary.push_back(QString::number(ops->inQueue));
            temporary.push_back(QString::number(idEquipment));
            if(!db->insertTable("Operations",temporary))                                    //Если не удалось записать данные в БД
            {
                QMessageBox::warning(this,"Ошибка при записи","Не удалось записать данные в таблицу Operations");
                break;
            }
            temporary.clear();
            i++;
        }
    }
}

void MainWindow::on_connectDB_clicked()                                                     //Слот для установки и разрыва соединения с БД
{
    if(!ui->LoginDB->text().isEmpty())                                                      //Если loginDB не пустой
    {
        if(!ui->PasswordDB->text().isEmpty())                                               //Если passwordDB не пустой
        {
            QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
            QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
            if(!ui->HostDB->text().isEmpty() && ipRegex.indexIn(ui->HostDB->text()) != -1)  //Если hostDB не пустой и соответствует регулярному выражению
            {
                if(!ui->NameDB->text().isEmpty())                                           //Если nameDB не пустой
                {
                    if(db->isOpen)                                                          //Если соединение установлено
                    {
                        db->closeConnection();
                        ui->connectDB->setText("Подключиться к БД");
                    }
                    else                                                                    //Если соединение не установлено
                    {
                        db->connection(ui->LoginDB->text(),ui->PasswordDB->text(),ui->HostDB->text(),ui->NameDB->text());
                        ui->connectDB->setText("Отключится от БД");
                    }
                }
                else QMessageBox::warning(this,"Ошибка","Введите название БД");
            }
            else QMessageBox::warning(this,"Ошибка","Введите IP");
        }
        else QMessageBox::warning(this,"Ошибка","Введите пароль");
    }
    else QMessageBox::warning(this,"Ошибка","Введите логин");
}
void MainWindow::on_InputDB_clicked()                                                       //Слот для записи данных в БД
{
    if(db->isOpen)                                                                          //Если соединение установлено
    {
        if(!scene->getOperations().isEmpty())                                               //Если на сцене есть операции
        {
            if(!ui->reserveBits->text().isEmpty())                                          //Если введены резервные биты
            {
                QRegExp bitRegex ("^[0-1]{1,128}$");
                if(bitRegex.indexIn(ui->reserveBits->text()) != -1)                         //Если резервные биты соответствуют регулярному выражению
                {
                    int idMoscowTime = addMoscowTimeInDB(ui->NameDB->text());
                    if(idMoscowTime != -1)
                    {
                        int idRelayCommands = addRelayCommandsInDB(ui->NameDB->text());
                        if(idRelayCommands != -1)
                        {
                            addEquipmentInDB(idMoscowTime,idRelayCommands);
                            addOperationsInDB(ui->NameDB->text());
                        }
                    }
                }
                else QMessageBox::warning(this,"Ошибка","Резервные биты введены неверно");
            }
            else QMessageBox::warning(this,"Ошибка","Введите резервные биты");
        }
        else QMessageBox::warning(this,"Ошибка","Добавьте операцию на сцену");
    }
    else QMessageBox::warning(this,"Ошибка","Не удалось записать данные в БД");
}

void MainWindow::on_OutputDB_clicked()                                                      //Слот для считывания данных из БД
{
    if(db->isOpen)                                                                          //Если соединение установлено
    {
        QStringList data;
        if(db->outputFromTable("Cyclogram","Operations",&data))                             //Если удалось считать данные из БД
        {
            QStringList line;
            foreach(QString str, data)
            {
                line = str.split(',');
                if(line.at(8).toInt() == idEquipment)                                       //Если считываемый idEquipment равен текущему idEquipment
                {
                    if(line.at(6) == "true")                                                //Если считываемый dynamic = true
                    {
                        if(line.at(7) == "true")                                            //Если считываемый inQueue = true
                            scene->addOperations(line.at(1),line.at(2).toDouble(),line.at(3).toDouble(),line.at(4).toDouble(),line.at(5).toDouble(),true,true);
                        else
                            scene->addOperations(line.at(1),line.at(2).toDouble(),line.at(3).toDouble(),line.at(4).toDouble(),line.at(5).toDouble(),true,false);
                    }
                    else
                    {
                        if(line.at(7) == "true")                                            //Если считываемый inQueue = true
                            scene->addOperations(line.at(1),line.at(2).toDouble(),line.at(3).toDouble(),line.at(4).toDouble(),line.at(5).toDouble(),false,true);
                        else
                            scene->addOperations(line.at(1),line.at(2).toDouble(),line.at(3).toDouble(),line.at(4).toDouble(),line.at(5).toDouble(),false,false);
                    }
                }
                line.clear();
            }
            ui->comboBox->clear();
            ui->comboBox_2->clear();
            foreach(QString name, scene->getNamesOperations())                              //Добавляем все название операций в combobox
            {
                ui->comboBox->addItem(name);
                ui->comboBox_2->addItem(name);
                ui->comboBox_5->addItem(name);
            }
        }
        else QMessageBox::warning(this,"Ошибка","Не удалось считать данные из БД");
    }
    else QMessageBox::warning(this,"Ошибка","Не удалось подключиться БД");
}

void MainWindow::on_OnOffcyclogram_clicked()                                                //Слот для включения или выключения циклограммы
{
    if(!clickOn)                                                                            //Если циклограма не включена
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
        ui->connectDB->setEnabled(true);
        scene->on();                                                                        //Отрисовываем начало циклограммы
        clickOn = true;
        ui->OnOffcyclogram->setText("Выкл");
    }
    else                                                                                    //Если циклограма включена
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
        ui->connectDB->setEnabled(false);
        ui->setMinWidthOperation->hide();
        ui->setMaxWidthOperation->hide();
        ui->comboBox_5->hide();
        scene->off();                                                                       //Отрисовываем конец циклограммы
        clickOn = false;
        clickOff = true;
        ui->OnOffcyclogram->setText("Вкл");
    }
}

void MainWindow::on_AddOperation_clicked()                                                  //Слот для добавления операции
{
    QRegExp rgx("\\d{1,3}");                                                                //Создаём регулярное выражение
    bool dynamic=true;
    if(rgx.indexIn(ui->coordX->text()) != -1 && rgx.indexIn(ui->coordY->text()) != -1 && rgx.indexIn(ui->operationWidth->text()) != -1 && rgx.indexIn(ui->operationHeight->text()) != -1)
    {                                                                                       //Проверка что вводятся цифры
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
               ui->operationHeight->text().toDouble() < 150)                                //Проверка чтобы вводимые данные не выходили за границы представления
            {
                if(ui->comboBox_4->currentIndex() == 0)                                     //Если выбрана динамическая операция
                {
                    dynamic = true;
                }
                else
                {
                    dynamic = false;
                }
                if(scene->addOperations(ui->nameOperations->text(),ui->coordX->text().toDouble(),ui->coordY->text().toDouble(),ui->operationWidth->text().toDouble(),ui->operationHeight->text().toDouble(),dynamic))
                {                                                                           //Если удалось добавить операцию
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

void MainWindow::on_UpdateOperation_clicked()                                               //Слот для обновления операций
{
    QRegExp rgx("\\d{1,3}");                                                                //Создаём регулярное выражение
    if(rgx.indexIn(ui->newCoordX->text()) != -1 || rgx.indexIn(ui->newCoordY->text()) != -1 || rgx.indexIn(ui->newOperationWidth->text()) != -1 || rgx.indexIn(ui->newOperationHeight->text()) != -1)
    {                                                                                       //Проверка что вводятся цифры
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

void MainWindow::on_DeleteOperation_clicked()                                              //Слот для удаления операций
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

void MainWindow::on_AddQueue_clicked()                                                      //Слот для добавления операций в очередь
{
    scene->addList(ui->comboBox_2->currentIndex(),ui->comboBox_3->currentText());
    scene->createQueue();
}

void MainWindow::on_InputFile_clicked()                                                     //Слот для записи данных в файл
{
    QString path = QFileDialog::getOpenFileName(this,"Выберите файл","","*.json");          //Получаем путь к файлу
    if(!path.isEmpty())                                                                     //Если путь к файлу не пустой
    {
        if(!scene->inFile(path))                                                            //Если не удалось записать в файл
            QMessageBox::warning(this,"Ошибка с файлом","Данный файл невозможно открыть.");
    }
    else
        QMessageBox::warning(this,"Путь к файлу","Путь к файлу указан неверно.");
}

void MainWindow::on_OutputFile_clicked()                                                    //Слот для считывания данных из файла
{
    QString path = QFileDialog::getOpenFileName(this,"Выберите файл","","*.json");          //Получаем путь к файлу
    if(!path.isEmpty())                                                                     //Если путь к файлу не пустой
    {
        if(!scene->outFile(path))                                                           //Если не удалось считать из файла
            QMessageBox::warning(this,"Ошибка с файлом","Данный файл невозможно открыть.");
        else
        {
            ui->comboBox->clear();
            ui->comboBox_2->clear();
            foreach(QString name, scene->getNamesOperations())                              //Заполняем combobox
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
