#ifndef ACTION_H
#define ACTION_H

#include <QDialog>
#include <QMessageBox>

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

private:
    Ui::Action *ui;

signals:
    void signalAddOperation(QString name, double widthOperation, double intervalOperations, bool dynamic);
    void signalUpdateOperation(int index, QString name, double widthOperation, double intervalOperations);
    void signalDeleteOperation(int index);
private slots:
    void on_AddOperation_clicked();
    void on_UpdateOperation_clicked();
    void on_DeleteOperation_clicked();
};

#endif // ACTION_H
