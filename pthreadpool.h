#ifndef PTHREADPOOL_H
#define PTHREADPOOL_H

#include <QObject>
#include <QRunnable> //线程池
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QFileDialog>
#include <QDateTime>
#include <QtGui/QScreen>
#include "mysql.h"
#include <QThreadPool>
#include "car.h"

//更新数据库预约数线程
class PReserve : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit PReserve(QObject *parent = nullptr);

    void recName(QSqlQuery q,QString name);
    //摄像头程序运行
    void run() override;
signals:
    void finish(int reserve,int now_reserve);
private:
    //创建数据库对象
    //mysql mysql_C;
    //数据库执行类
    QSqlQuery m_q;
    QString p_name;

public slots:
};

class INIT : public QObject
{
    Q_OBJECT
public:
    explicit INIT(QObject *parent = nullptr);

    //初始化数据库
    void working(mysql mysql_C);
signals:
    void finish();
private:



public slots:
};

//摄像头单线程

//class CameraThread: public QThread
//{
//    Q_OBJECT
//public:
//    CameraThread(QObject* parent = nullptr): QThread(parent)
//    {

//    }

//    void run() override;

//signals:
//    void cameraInitialized(QCamera*, QCameraImageCapture*);
//private:
//    //创建摄像头对象
//    QCamera *camera;
//    //显示摄像头的指针
//    QCameraViewfinder *viemfinder;
//    //捕获图片,操作用于捕获的对象
//    QCameraImageCapture *imageCapture;

//};

class Recognize : public QObject
{
    Q_OBJECT
public:
    explicit Recognize(QObject *parent = nullptr);

    //初始化数据库
    //传参图片帧
    void working(Mat rgbImg);
signals:
    //传参数字符串
    void finish(QImage qPlateImg,QString plate);
private:
    easypr::CPlateRecognize m_plateRecognize;


public slots:
};











#endif // PTHREADPOOL_H
