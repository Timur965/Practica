#include "secondwindow.h"
#include "Test/test_database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SecondWindow w;
    w.show();
    QTest::qExec(new Test_Database,argc,argv);
    return a.exec();
}
