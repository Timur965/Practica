#ifndef ACTION_H
#define ACTION_H

#include <QDialog>
#include <QMessageBox>
#include "fileinout.h"

namespace Ui {
class Action;
}

class Action : public QDialog
{
    Q_OBJECT

public:
    explicit Action(QWidget *parent = nullptr);
    ~Action();
    void completionCombobox(QVector<QString> data);
    void showAction(QString nameAction);
    void afterAdd(QString nameOperation);
    bool afterDelete();
    void completionAllOperations(Geometry geom);
    static int id;
private:
    Ui::Action *ui;
    FileInOut *file = nullptr;
    QVector<Geometry> *operations = nullptr;
    QVector<Geometry> *allOperations = nullptr;

signals:
    void signalAddOperation(QString name, QString reduction, double widthOperation, double intervalOperations, bool dynamic);
    void signalUpdateOperation(int index, QString name, QString reduction, double widthOperation, double intervalOperations);
    void signalDeleteOperation(int index);
private slots:
    void on_AddOperation_clicked();
    void on_UpdateOperation_clicked();
    void on_DeleteOperation_clicked();
    void on_comboBox_4_currentIndexChanged(int index);
    void on_comboBox_currentIndexChanged(const QString &arg1);
};

#endif // ACTION_H
