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

    connect(scene,&MyGraphicScene::increaseView,this,&MainWindow::updateIncreaseView);     //Соединение сигнала со слотом
    connect(scene,&MyGraphicScene::decreaseView,this,&MainWindow::updateDecreaseView);     //Соединение сигнала со слотом
    connect(scene,&MyGraphicScene::setSizeGraphicsView,this,&MainWindow::updateSizeView);  //Соединение сигнала со слотом

    window = new Action();
    connect(window,&Action::signalAddOperation,this,&MainWindow::addOperation);
    connect(window,&Action::signalUpdateOperation,this,&MainWindow::updateOperation);
    connect(window,&Action::signalDeleteOperation,this,&MainWindow::deleteOperation);

    ui->graphicsView->setEnabled(false);
    ui->AddOperation->setEnabled(false);
    ui->UpdateOperation->setEnabled(false);
    ui->DeleteOperation->setEnabled(false);
    ui->InputFile->setEnabled(false);
    ui->connectDB->setEnabled(false);
    ui->InputDB->setEnabled(false);
    ui->OutputDB->setEnabled(false);

    for(int i=1;i<=10;i++)
    {
        ui->number->addItem(QString::number(i));
    }
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
    if(window != nullptr)
        delete window;                                                                      //Освобождение памяти из под window
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)                                //Виртуальный метод для обработки события мыши
{
    scene->width = ui->graphicsView->width();
    scene->height = ui->graphicsView->height();
    return QObject::eventFilter(object,event);
}
void MainWindow::resizeEvent(QResizeEvent *)                                                //Определяем виртуальный метод для обработки события изменение окна
{
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

bool MainWindow::addOperationsInDB(QString nameDatabase)                                    //Добавление в таблицу БД
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
        int i=0;
        temporary.clear();
        foreach(Operation *ops, scene->getOperations())
        {
            temporary.push_back(QString::number(n+i));
            temporary.push_back(ops->name);
            temporary.push_back(ops->reduction);
            temporary.push_back(QString::number(ops->width/Operation::getCoef()));
            temporary.push_back(QString::number(ops->interval/Operation::getCoef()));
            temporary.push_back(QString::number(ops->dynamic));
            temporary.push_back(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            if(!db->insertTable("Operations",temporary))                                    //Если не удалось записать данные в БД
            {
                QMessageBox::warning(this,"Ошибка при записи","Не удалось записать данные в таблицу Operations");
                break;
            }
            temporary.clear();
            i++;
        }
        return true;
    }
    return false;
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
                        ui->DateTimeOutBD->clear();
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
                                   if(ui->DateTimeOutBD->findText(line.at(6)) == -1)
                                        ui->DateTimeOutBD->addItem(line.at(6));
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
            if(addOperationsInDB(ui->NameDB->text()))
            {
                QMessageBox::information(this,"Запись","Данные успешно записаны");
                QStringList data;
                if(db->outputFromTable("Cyclogram","Operations",&data))                     //Если удалось считать данные из БД
                {
                   QStringList line;
                   ui->DateTimeOutBD->clear();
                   foreach(QString str, data)
                   {
                       line = str.split(',');
                       if(ui->DateTimeOutBD->findText(line.at(6)) == -1)
                            ui->DateTimeOutBD->addItem(line.at(6));
                   }
                }
            }
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
            scene->allClear();
            window->afterDelete();
            ui->graphicsView->setMinimumWidth(0);
            ui->graphicsView->setMaximumWidth(925);
            foreach(QString str, data)
            {
                line = str.split(',');
                if(line.at(6) == ui->DateTimeOutBD->currentText())                          //Если считываемая дата равна выбранной
                {
                    if(line.at(5) == "true")                                                //Если считываемый dynamic = true
                    {
                        scene->addOperations(line.at(1),line.at(2),line.at(3).toDouble(),line.at(4).toDouble(),true);
                    }
                    else
                    {
                        scene->addOperations(line.at(1),line.at(2),line.at(3).toDouble(),line.at(4).toDouble(),false);
                    }
                    window->completionCombobox(scene->getNamesOperations());
                    if(scene->getNamesOperations().contains("Запись информации"))
                        window->afterAdd("Запись информации");
                    if(scene->getNamesOperations().contains("Воспроизведение информации"))
                        window->afterAdd("Воспроизведение информации");
                }
                line.clear();
            }
        }
        else QMessageBox::warning(this,"Ошибка","Не удалось считать данные из БД");
    }
    else QMessageBox::warning(this,"Ошибка","Не удалось подключиться БД");
}
void MainWindow::selectingAction(QString nameAction)
{
    window->showAction(nameAction);
    window->completionCombobox(scene->getNamesOperations());
    window->setModal(true);
    window->show();
}
void MainWindow::on_AddOperation_clicked()                                                  //Слот для добавления операции
{
    selectingAction("Добавить");
}
void MainWindow::addOperation(QString name, QString reduction, double widthOperation, double intervalOperations, bool dynamic)
{
    if(scene->addOperations(name,reduction,widthOperation,intervalOperations,dynamic))
    {                                                                                       //Если удалось добавить операцию
        window->completionCombobox(scene->getNamesOperations());
        if(scene->getNamesOperations().contains("Запись информации"))
            window->afterAdd("Запись информации");
        if(scene->getNamesOperations().contains("Воспроизведение информации"))
            window->afterAdd("Воспроизведение информации");
    }
    else QMessageBox::warning(this,"Добавление","Имя занято.");
}
void MainWindow::on_UpdateOperation_clicked()                                               //Слот для обновления операций
{
    selectingAction("Изменить");
}
void MainWindow::updateOperation(int index, QString name, QString reduction, double widthOperation, double intervalOperations)
{
    if(scene->updateOperations(index,name,reduction,widthOperation,intervalOperations))
    {
        window->completionCombobox(scene->getNamesOperations());
    }
    else QMessageBox::warning(this,"Изменение","Имя занято.");
}
void MainWindow::on_DeleteOperation_clicked()                                               //Слот для удаления операций
{
    selectingAction("Удалить");
}
void MainWindow::deleteOperation(int index)
{
    if(scene->deleteOperations(index))
    {
        window->completionCombobox(scene->getNamesOperations());
        if(!scene->getNamesOperations().contains("Запись информации")|| !scene->getNamesOperations().contains("Воспроизведение информации"))
            window->afterDelete();
    }
    else QMessageBox::warning(this,"Удаление","Не получилось удалить.");

}
void MainWindow::on_InputFile_clicked()                                                     //Слот для записи данных в файл
{
    QDir dir;
    dir.mkdir(QApplication::applicationDirPath()+"\\Циклограмма");
    QString path;
    if(scene->getNamesOperations().indexOf("Запись информации") != -1)
    {
        path = QFileDialog::getSaveFileName(this,"Выберите файл",QString(QApplication::applicationDirPath()+"\\Циклограмма\\Сеанс №%1 %2 %3").arg(ui->number->currentText(),"Запись информации",QDateTime::currentDateTime().toString("dd-MM-yyyy HH-mm-ss")),tr("*.json"));
                                                                                            //Получаем путь к файлу
    }
    else
    {
        if(scene->getNamesOperations().indexOf("Воспроизведение информации") != -1)
            path = QFileDialog::getSaveFileName(this,"Выберите файл",QString(QApplication::applicationDirPath()+"\\Циклограмма\\Сеанс №%1 %2 %3").arg(ui->number->currentText(),"Воспроизведение информации",QDateTime::currentDateTime().toString("dd-MM-yyyy HH-mm-ss")),tr("*.json"));
                                                                                                //Получаем путь к файлу
        else
            path = QFileDialog::getSaveFileName(this,"Выберите файл",QString(QApplication::applicationDirPath()+"\\Циклограмма"),tr("*.json"));
    }
    if(!path.isEmpty())                                                                     //Если путь к файлу не пустой
    {
        if(!scene->getOperations().isEmpty())                                               //Если сцена не пустая
        {
            setWindowTitle("Разработка циклограммы "+path.split('/').back());
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

bool MainWindow::OutputFile()                                                               //Слот для считывания данных из файла
{
    QString path = QFileDialog::getOpenFileName(this,"Выберите файл",QApplication::applicationDirPath()+"\\Циклограмма",tr("*.json"));      //Получаем путь к файлу
    if(!path.isEmpty())                                                                     //Если путь к файлу не пустой
    {
        vectorGeometry = new QVector<Geometry>();
        fileInOut->outputJSONFile(path,vectorGeometry);                                     //Считываем данные из файла
        if(!vectorGeometry->empty())
        {
            setWindowTitle("Разработка циклограммы "+path.split('/').back());
            scene->allClear();
            window->afterDelete();
            ui->graphicsView->setMinimumWidth(0);
            ui->graphicsView->setMaximumWidth(925);
            foreach(Geometry geom, *vectorGeometry)
            {
                scene->addOperations(geom.name,geom.reduction,geom.width,geom.interval,geom.dynamic);
                                                                                            //Добавляем на сцену
            }
            window->completionCombobox(scene->getNamesOperations());
            if(scene->getNamesOperations().contains("Запись информации"))
                window->afterAdd("Запись информации");
            if(scene->getNamesOperations().contains("Воспроизведение информации"))
                window->afterAdd("Воспроизведение информации");
            scene->createQueue();
            return true;
        }
        else
            QMessageBox::warning(this,"Ошибка","Не удалось считать данные из файл");
    }
    else
        QMessageBox::warning(this,"Путь к файлу","Путь к файлу указан неверно.");
    return false;
}

void MainWindow::on_createCyclogram_triggered()
{
    ui->graphicsView->setEnabled(true);
    ui->AddOperation->setEnabled(true);
    ui->UpdateOperation->setEnabled(true);
    ui->DeleteOperation->setEnabled(true);
    ui->InputFile->setEnabled(true);
    ui->connectDB->setEnabled(true);
    ui->InputDB->setEnabled(true);
    ui->OutputDB->setEnabled(true);
    scene->allClear();
    window->afterDelete();
    updateSizeView();
    scene->update();
}

void MainWindow::on_updateCyclogram_triggered()
{
    if(OutputFile())
    {
        ui->graphicsView->setEnabled(true);
        ui->AddOperation->setEnabled(true);
        ui->UpdateOperation->setEnabled(true);
        ui->DeleteOperation->setEnabled(true);
        ui->InputFile->setEnabled(true);
        ui->connectDB->setEnabled(true);
        ui->InputDB->setEnabled(true);
        ui->OutputDB->setEnabled(true);
    }
    scene->update();
}

void MainWindow::on_showCyclogram_triggered()
{
    if(OutputFile())
    {
        ui->graphicsView->setEnabled(false);
        ui->AddOperation->setEnabled(false);
        ui->UpdateOperation->setEnabled(false);
        ui->DeleteOperation->setEnabled(false);
        ui->InputFile->setEnabled(false);
        ui->connectDB->setEnabled(false);
        ui->InputDB->setEnabled(false);
        ui->OutputDB->setEnabled(false);
    }
    scene->update();
}
