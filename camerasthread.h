#ifndef CAMERASTHREAD_H
#define CAMERASTHREAD_H

#include <QObject>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QThread>
#include <QTime>
#include <QApplication>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/types_c.h"
#include "iostream"
using namespace std;
using namespace cv;

//相机类：提供相机的基本操作
class Camera : public QObject
{
    Q_OBJECT

public:
    Camera();
    ~Camera();
    bool open(int _index); //判断当前摄像头是否处于打开状态
    void close();  //关闭当前摄像头
    Mat read();
    Mat matnow;
    QImage Mat2QImage(Mat const& src); //此引用所代表的Mat类型变量无法使用此引用修改.
    Mat QImage2Mat(QImage const& src);
    static int getCameraCount();
signals:
    void updateImage(QImage const&);
public slots:
    void Operate(int); //当Camera类被MovetoThread时，该槽函数相当于QThread::run()
private:
    VideoCapture capture;  //视频流捕获器
    static int cameracount; //存储相机数目
};
#endif // CAMERASTHREAD_H
