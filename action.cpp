#include "action.h"
#include "ui_action.h"

Action::Action(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Action)
{
    ui->setupUi(this);

    ui->comboBox_4->addItem("Динамический");
    ui->comboBox_4->addItem("Разовая команда");

    if(namesOperations == nullptr)
    {
        namesOperations = new QStringList();
        reductionOperations = new QStringList();
        file = new FileInOut();
        if(file->outputNamesOperation(QDir::currentPath()+"\\Циклограмма\\Названия операций.json", namesOperations, reductionOperations))
        {
            foreach(QString str, *namesOperations)
            {
                ui->nameOperations->addItem(str);
                ui->nameOperations_2->addItem(str);
            }
        }
        else QMessageBox::warning(this,"Ошибка","Не удалось считать названия операций");
    }
}

Action::~Action()
{
    delete ui;
    if(file != nullptr)
        delete file;
    if(namesOperations != nullptr)
        delete namesOperations;
    if(reductionOperations != nullptr)
        delete reductionOperations;
}
void Action::completionCombobox(QVector<QString> data)
{
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    foreach(QString name, data)                                                             //Заполняем combobox
    {
        ui->comboBox->addItem(name);
        ui->comboBox_2->addItem(name);
    }
}
void Action::showAction(QString nameAction)
{
    if(nameAction == "Добавить")
    {
        ui->Add->show();
        ui->Update->hide();
        ui->Delete->hide();
        setWindowTitle("Добавление");                                                       //Устанавливаем заголовок окну
    }
    if(nameAction == "Изменить")
    {
        ui->Add->hide();
        ui->Update->show();
        ui->Delete->hide();
        setWindowTitle("Изменение");                                                        //Устанавливаем заголовок окну
    }
    if(nameAction == "Удалить")
    {
        ui->Add->hide();
        ui->Update->hide();
        ui->Delete->show();
        setWindowTitle("Удаление");                                                         //Устанавливаем заголовок окну
    }
}
void Action::on_AddOperation_clicked()
{
    QRegExp rgx("\\d{1,3}");                                                                //Создаём регулярное выражение
    bool dynamic=true;
    if(rgx.indexIn(ui->operationWidth->text()) != -1 && rgx.indexIn(ui->operationInterval->text()) != -1)
    {                                                                                       //Проверка что вводятся цифры
        if(ui->operationWidth->text().toDouble() >=1 &&
           ui->operationWidth->text().toDouble() <= 360 &&
           ui->operationInterval->text().toDouble() >= 0 &&
           ui->operationInterval->text().toDouble() < 360)                                  //Проверка чтобы вводимые данные не выходили за границы представления
        {
            if(ui->comboBox_4->currentIndex() == 0)                                         //Если выбрана динамическая операция
            {
                dynamic = true;
            }
            else
            {
                dynamic = false;
            }
            emit signalAddOperation(ui->nameOperations->currentText(),reductionOperations->at(ui->nameOperations->currentIndex()),ui->operationWidth->text().toDouble(),ui->operationInterval->text().toDouble(),dynamic);
        }
        else QMessageBox::warning(this,"Неккоректные данные","Вы ввели слишком большие или\n слишко малые координаты.");
    }
    else QMessageBox::warning(this,"Неккоректные данные","Вводить можно только цифры");
}

void Action::on_UpdateOperation_clicked()
{
    QRegExp rgx("\\d{1,3}");                                                                //Создаём регулярное выражение
    if(rgx.indexIn(ui->operationWidth_2->text()) != -1 || rgx.indexIn(ui->operationInterval_2->text()) != -1)
    {                                                                                       //Проверка что вводятся цифры
        rgx.setPattern("^[А-Я]|[а-я]$");
        if(ui->operationWidth_2->text().toDouble() >=1 &&
           ui->operationWidth_2->text().toDouble() <= 360 &&
           ui->operationInterval_2->text().toDouble() >=0 &&
           ui->operationInterval_2->text().toDouble() <= 360)                               //Проверка чтобы вводимые данные не выходили за границы представления
        {
            if(ui->comboBox->count() != NULL)                                               //Проверка если количество записей в combobox не равно 0
            {
                emit signalUpdateOperation(ui->comboBox->currentIndex(),ui->nameOperations_2->currentText(),reductionOperations->at(ui->nameOperations_2->currentIndex()),ui->operationWidth_2->text().toDouble(),ui->operationInterval_2->text().toDouble());
            }
            else QMessageBox::warning(this,"Неккоректные данные","Выберите операцию для изменения");
        }
        else QMessageBox::warning(this,"Неккоректные данные","Вы ввели слишком большие или\n слишко малые координаты.");
    }
    else QMessageBox::warning(this,"Неккоректные данные","Вводить можно только цифры");
}

void Action::on_DeleteOperation_clicked()
{
    if(ui->comboBox_2->count() != NULL)                                                       //Проверка если количество записей в combobox не равно 0
    {
        int index = ui->comboBox_2->currentIndex();
        ui->comboBox->removeItem(index);
        ui->comboBox_2->removeItem(index);
        emit signalDeleteOperation(index);
    }
    else
    {
        QMessageBox::warning(this,"Неккоректные данные","Выберите операцию для удаления");
    }
}

void Action::on_comboBox_4_currentIndexChanged(int index)
{
    if(index==0)
    {
        ui->operationWidth->show();
        ui->operationWidth->setText("");
        ui->label_3->show();
    }
    if(index==1)
    {
        ui->operationWidth->hide();
        ui->operationWidth->setText("1");
        ui->label_3->hide();
    }
}
