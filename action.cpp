#include "action.h"
#include "ui_action.h"

int Action::id = 0;
Action::Action(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Action)
{
    ui->setupUi(this);

    if(operations == nullptr)
    {
        operations = new QVector<Geometry>();
        allOperations = new QVector<Geometry>();
        file = new FileInOut();
        if(!afterDelete())
            QMessageBox::warning(this,"Ошибка","Не удалось считать названия операций");
    }

    ui->comboBox_4->addItem("Динамический");
    ui->comboBox_4->addItem("Разовая команда");
    connect(ui->comboBox_4,SIGNAL(currentIndexChanged(int)),this,SLOT(choiceOperations(int)));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(showLineEdit(QString)));
}

Action::~Action()
{
    delete ui;
    if(file != nullptr)
        delete file;
    if(operations != nullptr)
        delete operations;
    if(allOperations != nullptr)
        delete allOperations;
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
void Action::completionAllOperations(Geometry geom)
{
    if(geom.reduction.contains("ЗИ"))
        id++;
    allOperations->push_back(geom);
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
void Action::afterAdd(QString nameOperation)
{
    int i=0;
    if(nameOperation.contains("Воспроизведение информации"))
    {
        foreach(Geometry geom, *operations)
        {
            if(geom.name.contains("Запись информации"))
                operations->removeAt(i);
            i++;
        }
    }
    choiceOperations(ui->comboBox_4->currentIndex());
}
bool Action::afterDelete()
{
    if(file!=nullptr)
    {
        operations->clear();
        if(file->outputNamesOperation(QApplication::applicationDirPath()+"\\Циклограмма\\Названия операций.json", operations))
        {
            foreach(Geometry geom, *operations)
            {
                ui->nameOperations->addItem(geom.name);
            }
            choiceOperations(ui->comboBox_4->currentIndex());
            return true;
        }
    }
    return false;
}
void Action::on_AddOperation_clicked()
{
    QRegExp rgx("\\d{1,3}");                                                                //Создаём регулярное выражение
    bool dynamic=true;
    if(rgx.indexIn(ui->operationWidth->text()) != -1 && rgx.indexIn(ui->operationInterval->text()) != -1)
    {                                                                                       //Проверка что вводятся цифры
        if(ui->operationWidth->text().toDouble() >=1 &&
           ui->operationWidth->text().toDouble() <= 360 &&
           ((ui->nameOperations->currentText() != "Т0" && ui->operationInterval->text().toDouble() >= 1) ||
           (ui->nameOperations->currentText() == "Т0" && ui->operationInterval->text().toDouble() >= 0)) &&
           ui->operationInterval->text().toDouble() <= 360)                                 //Проверка чтобы вводимые данные не выходили за границы представления
        {
            if(ui->comboBox_4->currentIndex() == 0)                                         //Если выбрана динамическая операция
            {
                dynamic = true;
            }
            else
            {
                dynamic = false;
            }
            int index = -1;
            int i = 0;
            foreach(Geometry ops, *operations)
            {
                if(ops.name == ui->nameOperations->currentText())
                {
                    index = i;
                }
                i++;
            }
            Geometry op;
            if(operations->at(index).reduction.contains("ЗИ"))
            {
                if(id == 0)
                {
                    op.name = ui->nameOperations->currentText();
                    op.reduction = operations->at(index).reduction;
                }
                else
                {
                    op.name = ui->nameOperations->currentText()+QString::number(id);
                    op.reduction = operations->at(index).reduction+QString::number(id);
                }
                id++;
            }
            else
            {
                op.name = ui->nameOperations->currentText();
                op.reduction = operations->at(index).reduction;
            }
            op.width = ui->operationWidth->text().toDouble();
            op.interval = ui->operationInterval->text().toDouble();
            op.dynamic = dynamic;
            allOperations->push_back(op);
            if(index != -1)
                emit signalAddOperation(op.name,op.reduction,op.width,op.interval,op.dynamic);
            else
                QMessageBox::warning(this,"Ошибка","Такой операции не существует");
        }
        else
        {
            QMessageBox::warning(this,"Неккоректные данные","Вы ввели слишком большие или\n слишко малые координаты.");
        }
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
           ((ui->comboBox->currentText() != "Т0" && ui->operationInterval_2->text().toDouble() >= 1) ||
           (ui->comboBox->currentText() == "Т0" && ui->operationInterval_2->text().toDouble() >= 0)) &&
           ui->operationInterval_2->text().toDouble() <= 360)                               //Проверка чтобы вводимые данные не выходили за границы представления
        {
            if(ui->comboBox->count() != NULL)                                               //Проверка если количество записей в combobox не равно 0
            {
                int index = -1;
                int i = 0;
                foreach(Geometry ops, *allOperations)
                {
                    if(ops.name == ui->comboBox->currentText())
                    {
                        index = i;
                    }
                    i++;
                }
                if(index != -1)
                    emit signalUpdateOperation(ui->comboBox->currentIndex(),ui->comboBox->currentText(),allOperations->at(index).reduction,ui->operationWidth_2->text().toDouble(),ui->operationInterval_2->text().toDouble());
                else
                    QMessageBox::warning(this,"Ошибка","Такой операции не существует");

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
        if(index != -1)
        {
            ui->comboBox->removeItem(index);
            ui->comboBox_2->removeItem(index);
            allOperations->removeAt(index);
            id--;
            emit signalDeleteOperation(index);
        }
        else
            QMessageBox::warning(this,"Ошибка","Такой операции не существует");
    }
    else
    {
        QMessageBox::warning(this,"Неккоректные данные","Выберите операцию для удаления");
    }
}

void Action::choiceOperations(int index)
{
    if(index == 0)
    {
        ui->operationWidth->show();
        ui->operationWidth->setText("");
        ui->label_3->show();

        ui->nameOperations->clear();
        foreach(Geometry ops, *operations)
        {
            if(ops.dynamic)
            {
                ui->nameOperations->addItem(ops.name);
            }
        }

    }
    if(index == 1)
    {
        ui->operationWidth->hide();
        ui->operationWidth->setText("1");
        ui->label_3->hide();

        ui->nameOperations->clear();
        foreach(Geometry ops, *operations)
        {
            if(!ops.dynamic)
            {
                ui->nameOperations->addItem(ops.name);
            }
        }
    }
}

void Action::showLineEdit(const QString &arg1)
{
    foreach(Geometry ops, *allOperations)
    {
        if(arg1.contains(ops.name))
        {
            if(ops.dynamic)
            {
                ui->operationWidth_2->show();
                ui->operationWidth_2->setText("");
                ui->label_5->show();
            }
            else
            {
                ui->operationWidth_2->hide();
                ui->operationWidth_2->setText("1");
                ui->label_5->hide();
            }
        }
    }
}
