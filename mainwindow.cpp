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
    connect(scene,&MyGraphicScene::setSizeGraphicsView,this,&MainWindow::updateSizeView);  //Соединение сигнала со слотом

    ui->comboBox_4->addItem("Динамический");
    ui->comboBox_4->addItem("Разовая команда");
}

MainWindow::~MainWindow()
{
    delete ui;
    if(scene != nullptr)
        delete scene;                                                                       //Освобождение памяти из под scene
    if(db != nullptr)
        delete db;                                                                          //Освобождение памяти из под db
    if(fileInOut != nullptr)
        delete fileInOut;                                                                   //Освобождение памяти из под fileInOut
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
                if(scene->addOperations("",0,0,60,0,dynamic))
                {
                    ui->comboBox->addItem(scene->getCurrentName());
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
    scene->processingRelease();
    scene->createQueue();
}

void MainWindow::updateIncreaseView()                                                       //Слот для увелечения представления
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() + 240);                     //Устанавливаем минимальный размер представления
    ui->graphicsView->setMaximumWidth(ui->graphicsView->width() + 240);                     //Устанавливаем минимальный размер представления
    scene->createQueue();                                                                   //Обновляем координаты операций в очереди
}
void MainWindow::updateDecreaseView()                                                       //Слот для уменьшения представления
{
    ui->graphicsView->setMinimumWidth(ui->graphicsView->width() - 240);                     //Устанавливаем минимальный размер представления
    ui->graphicsView->setMaximumWidth(ui->graphicsView->width() - 240);                     //Устанавливаем минимальный размер представления
    scene->createQueue();                                                                   //Обновляем координаты операций в очереди
}
void MainWindow::updateSizeView()
{
    ui->graphicsView->setMinimumWidth(0);                                                   //Устанавливаем минимальный размер представления
    ui->graphicsView->setMaximumWidth(925);                                                 //Устанавливаем минимальный размер представления
    scene->createQueue();                                                                   //Обновляем координаты операций в очереди
}

void MainWindow::mode(QString nameMode)                                                     //Метод для обработки выбранного режима аппаратуры
{
    if(nameMode == "Запись информации")
    {
        scene->recordingInformation();                                                      //Вызываем метод для отрисовки плана "Запись информации"
        this->nameMode = nameMode;
        foreach(QString name, scene->getNamesOperations())                                  //Записываем все названия операций в combobox
        {
            ui->comboBox->addItem(name);
        }
    }
}
void MainWindow::onTime(QString dateTime)                                                   //Метод для получения вида аппаратуры
{
    this->dateTime = dateTime.split(": ").back();
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
            if(i == temporary.count() - 1)
            {
                n = temporary.at(i).split(",").at(0).toInt() + 1;                           //Устанавливаем последний id из БД
            }
        }
        int i=1;
        temporary.clear();
        foreach(Operation *ops, scene->getListOperations())
        {
            temporary.push_back(QString::number(n+i));
            temporary.push_back(ops->name);
            temporary.push_back(QString::number(ops->pos().x()));
            temporary.push_back(QString::number(ops->pos().y()));
            temporary.push_back(QString::number(ops->width/Operation::getCoef()));
            temporary.push_back(QString::number(ops->interval/Operation::getCoef()));
            temporary.push_back(QString::number(ops->dynamic));
            temporary.push_back(QString::number(ops->inQueue));
            temporary.push_back(nameMode);
            temporary.push_back(dateTime);
            if(!db->insertTable("Operations",temporary))                                    //Если не удалось записать данные в БД
            {
                QMessageBox::warning(this,"Ошибка при записи","Не удалось записать данные в таблицу Operations");
                break;
            }
            temporary.clear();
            i++;
        }
        temporary.clear();
        foreach(Operation *ops, scene->getOperations())
        {
            if(!ops->inQueue)
            {
                temporary.push_back(QString::number(n+i));
                temporary.push_back(ops->name);
                temporary.push_back(QString::number(ops->pos().x()));
                temporary.push_back(QString::number(ops->pos().y()));
                temporary.push_back(QString::number(ops->width/Operation::getCoef()));
                temporary.push_back(QString::number(ops->interval/Operation::getCoef()));
                temporary.push_back(QString::number(ops->dynamic));
                temporary.push_back(QString::number(ops->inQueue));
                temporary.push_back(nameMode);
                temporary.push_back(dateTime);
                if(!db->insertTable("Operations",temporary))                                    //Если не удалось записать данные в БД
                {
                    QMessageBox::warning(this,"Ошибка при записи","Не удалось записать данные в таблицу Operations");
                    break;
                }
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
                        if(!db->connection(ui->LoginDB->text(),ui->PasswordDB->text(),ui->HostDB->text(),ui->NameDB->text()))
                        {
                            QMessageBox::warning(this,"Ошибка","Не удалось подключиться к БД");
                        }
                        else
                        {
                            QStringList data;
                            if(db->outputFromTable("Cyclogram","Operations",&data))                                 //Если удалось считать данные из БД
                            {
                               QStringList line;
                               ui->DateTimeOutBD->clear();
                               foreach(QString str, data)
                               {
                                   line = str.split(',');
                                   if(ui->DateTimeOutBD->findText(line.at(9)) == -1)
                                        ui->DateTimeOutBD->addItem(line.at(9));
                               }
                            }
                            ui->connectDB->setText("Отключиться от БД");
                        }
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
            addOperationsInDB(ui->NameDB->text());
            QMessageBox::information(this,"Запись","Данные успешно записаны");
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
                if(line.at(9) == ui->DateTimeOutBD->currentText() && line.at(8) == nameMode)//Если считываемая дата равна выбранной и режим равен выбранному
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
            foreach(QString name, scene->getNamesOperations())                              //Добавляем все название операций в combobox
            {
                ui->comboBox->addItem(name);
            }
        }
        else QMessageBox::warning(this,"Ошибка","Не удалось считать данные из БД");
    }
    else QMessageBox::warning(this,"Ошибка","Не удалось подключиться БД");
}

void MainWindow::on_AddOperation_clicked()                                                  //Слот для добавления операции
{
    QRegExp rgx("\\d{1,3}");                                                                //Создаём регулярное выражение
    bool dynamic=true;
    if(rgx.indexIn(ui->coordX->text()) != -1 && rgx.indexIn(ui->coordY->text()) != -1 && rgx.indexIn(ui->operationWidth->text()) != -1 && rgx.indexIn(ui->operationInterval->text()) != -1)
    {                                                                                       //Проверка что вводятся цифры
        rgx.setPattern("^[А-Я]|[а-я]$");
        if(rgx.indexIn(ui->nameOperations->text()) != -1)
        {
            if(ui->coordX->text().toDouble() > (ui->graphicsView->width()/2 - ui->graphicsView->width())+55 &&
               ui->coordX->text().toDouble() < ui->graphicsView->width()/2 - 40 &&
               ui->coordY->text().toDouble() > (ui->graphicsView->height()/2 - ui->graphicsView->height())+80 &&
               ui->coordY->text().toDouble() < ui->graphicsView->height()/2 - 45 &&
               ui->operationWidth->text().toDouble() >=1 &&
               ui->operationWidth->text().toDouble() <= 70 &&
               ui->operationInterval->text().toDouble() >= 0 &&
               ui->operationInterval->text().toDouble() < 60)                                //Проверка чтобы вводимые данные не выходили за границы представления
            {
                if(ui->comboBox_4->currentIndex() == 0)                                     //Если выбрана динамическая операция
                {
                    dynamic = true;
                }
                else
                {
                    dynamic = false;
                }
                if(scene->addOperations(ui->nameOperations->text(),ui->coordX->text().toDouble(),ui->coordY->text().toDouble(),ui->operationWidth->text().toDouble(),ui->operationInterval->text().toDouble(),dynamic))
                {                                                                           //Если удалось добавить операцию
                    ui->comboBox->addItem(scene->getCurrentName());
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
    if(rgx.indexIn(ui->coordX->text()) != -1 || rgx.indexIn(ui->coordY->text()) != -1 || rgx.indexIn(ui->operationWidth->text()) != -1 || rgx.indexIn(ui->operationInterval->text()) != -1)
    {                                                                                       //Проверка что вводятся цифры
        rgx.setPattern("^[А-Я]|[а-я]$");
        if(rgx.indexIn(ui->nameOperations->text()) != -1)
        {
            if(ui->coordX->text().toDouble() > (ui->graphicsView->width()/2 - ui->graphicsView->width())+55 &&
               ui->coordX->text().toDouble() < ui->graphicsView->width()/2 - 40 &&
               ui->coordY->text().toDouble() > (ui->graphicsView->height()/2 - ui->graphicsView->height())+80 &&
               ui->coordY->text().toDouble() < ui->graphicsView->height()/2 - 45 &&
               ui->operationWidth->text().toDouble() >=1 &&
               ui->operationWidth->text().toDouble() <= 70 &&
               ui->operationInterval->text().toDouble() >=0 &&
               ui->operationInterval->text().toDouble() <= 60)                           //Проверка чтобы вводимые данные не выходили за границы представления
            {
                if(ui->comboBox->count() != NULL)                                           //Проверка если количество записей в combobox не равно 0
                {
                    if(!scene->updateOperations(ui->comboBox->currentIndex(),ui->nameOperations->text(),ui->coordX->text().toDouble(),ui->coordY->text().toDouble(),ui->operationWidth->text().toDouble(),ui->operationInterval->text().toDouble()))
                    {
                        QMessageBox::warning(this,"Изменение","Координаты заняты другим объектом.");
                    }
                    else
                    {
                        ui->comboBox->clear();
                        foreach(QString name, scene->getNamesOperations())                  //Заполняем combobox
                        {
                            ui->comboBox->addItem(name);
                        }
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

void MainWindow::on_DeleteOperation_clicked()                                               //Слот для удаления операций
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

void MainWindow::on_AddQueue_clicked()                                                      //Слот для добавления операций в очередь
{
    scene->addList(ui->comboBox->currentIndex(),ui->comboBox_3->currentText());
    scene->createQueue();
}

void MainWindow::on_InputFile_clicked()                                                     //Слот для записи данных в файл
{
    QString path = QFileDialog::getOpenFileName(this,"Выберите файл","","*.json");          //Получаем путь к файлу
    if(!path.isEmpty())                                                                     //Если путь к файлу не пустой
    {
        if(!scene->getOperations().isEmpty())                                               //Если сцена не пустая
        {
            if(!fileInOut->inputJSONFile(scene->getOperations(),path))                      //Если не удалось записать в файл
            {
                 QMessageBox::warning(this,"Ошибка","Не удалось записать в файл");
            }
            else
            {
                QMessageBox::information(this,"Запись","Данные успешно записаны");
            }
        }
        else
             QMessageBox::warning(this,"Ошибка","Сцена пустая");
    }
    else
        QMessageBox::warning(this,"Путь к файлу","Путь к файлу указан неверно");
}

void MainWindow::on_OutputFile_clicked()                                                    //Слот для считывания данных из файла
{
    QString path = QFileDialog::getOpenFileName(this,"Выберите файл","","*.json");          //Получаем путь к файлу
    if(!path.isEmpty())                                                                     //Если путь к файлу не пустой
    {
        vectorGeometry = new QVector<Geometry>();
        fileInOut->outputJSONFile(path,vectorGeometry);                                     //Считываем данные из файла
        if(!vectorGeometry->empty())
        {
            foreach(Geometry geom, *vectorGeometry)
            {
                scene->addOperations(geom.name,geom.x,geom.y,geom.width,geom.interval,geom.dynamic,geom.inQueue);
                                                                                            //Добавляем на сцену
            }
            scene->createQueue(59,60);
            ui->comboBox->clear();
            foreach(QString name, scene->getNamesOperations())                              //Заполняем combobox
            {
                ui->comboBox->addItem(name);
            }
        }
        else
            QMessageBox::warning(this,"Ошибка","Не удалось считать данные из файл");
    }
    else
        QMessageBox::warning(this,"Путь к файлу","Путь к файлу указан неверно.");
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->operationInterval->hide();
        ui->operationWidth->show();
        ui->operationInterval->setText("0");
        ui->operationWidth->setText("");

        ui->label_8->hide();
        ui->label_3->show();
    }
    else
    {
        ui->operationInterval->show();
        ui->operationWidth->hide();
        ui->operationWidth->setText("1");
        ui->operationInterval->setText("");

        ui->label_8->show();
        ui->label_3->hide();
    }
}
