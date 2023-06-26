#include "login.h"
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
        qDebug()<<"文件不存在，初始化!";
        initFile *initmysql_file = new initFile;
        initmysql_file->show();
        file.close();
    }
    else
    {
        //文件存在启动主程序
        MainWindow *w = new MainWindow;
        w->show();
        file.close();
        w->setAttribute(Qt::WA_DeleteOnClose);

    }
    return a.exec();
}
