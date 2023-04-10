#include "mainwindow.h"
#include <QSqlDatabase>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //判断是否初始化


    MainWindow w;
    w.show();
    return a.exec();
}
