#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QMessageBox::information(0,"info","Enter source code in source code field and enter SUBMIT button");

    return a.exec();
}
