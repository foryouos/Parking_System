#include "mainwindow.h"
#include <QSqlDatabase>
#include <QApplication>
//MySQL数据库初始化函数





int main(int argc, char *argv[])
{
    //mysql_Init();
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
