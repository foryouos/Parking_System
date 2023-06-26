#ifndef CAMERASTHREAD_H
#define CAMERASTHREAD_H
#include <QMediaPlayer>
#include <QVideoWidget>

#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QFileDialog>
#include <QDateTime>
#include <QtGui/QScreen>
#include <QObject>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QThread>
#include "iostream"

using namespace std;

// TODO纯Qcamera 方案舍弃

//相机类：提供相机的基本操作
class Camera : public QObject
{
    Q_OBJECT

public:
    Camera(int number);  //传入初始化的摄像头编号
    ~Camera();

    static int getCameraCount();
signals:
    void Camera_Image_signal(int id,const QImage &preview);
public slots:

    void CameraShow();

private:

    //创建摄像头对象
    QCamera *camera;

    //捕获图片,操作用于捕获的对象
    QCameraImageCapture *imageCapture;
    bool camera_statue = false;
};
#endif // CAMERASTHREAD_H
