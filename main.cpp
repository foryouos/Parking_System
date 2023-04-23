#include "mainwindow.h"

#include <QSqlDatabase>
#include <QApplication>
#include "initfile.h"
#include <opencv2/opencv.hpp>
#include <QCoreApplication>
int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    QApplication a(argc, argv);

    //判断是否存在初始化文件若不存在启动，初始化程序
    QString filePath =QApplication::applicationDirPath() + "/parkinginit.json";
    QFile file(filePath);
    //如果文件不能打开不存在
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //创建登陆主页面
        //Car * w =new Car;
        //呈现Car主页面
        //w->show();
        qDebug()<<"文件不存在，初始化!";
        initFile *i = new initFile;
        i->show();
    }
    else
    {
        //文件存在启动主程序
        MainWindow *w = new MainWindow;
        w->show();
    }




    return a.exec();
}
