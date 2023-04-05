#include "mainwindow.h"
#include <QSqlDatabase>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //采用动态加载UI文件并生成界面


    return a.exec();
}
