#include "camerasthread.h"
int Camera::cameracount=0; //类体外部初始化静态成员
Camera::Camera()
{
}

Camera::~Camera()
{
   if(!capture.isOpened())capture.release();
}

bool Camera::open(int _index)
{
    qDebug()<<"open index= "<<_index;
    if(capture.open(_index,CAP_DSHOW))
        return true;
    else
        return false;
}

void Camera::close()
{
    capture.release();
}

Mat Camera::read()
{
    Mat mat;
    capture.read(mat);
    return mat;
}

void Camera::Operate(int _index)
{
    if(open(_index)) //如果当前摄像头处于打开模式，成功
    {
        qDebug()<<"Camera open success!";
    }
    else
    {
        qDebug()<<"Camera open failed!";
        return;
    }
    while(1)
    {
        qApp->processEvents();
        Mat matin=read(); //视频流读入
        matnow=matin;
        QImage image=Mat2QImage(matin);
        emit updateImage(image);
    }
}

QImage Camera::Mat2QImage(Mat const& mat)
{
    Mat temp;
    cvtColor(mat, temp,CV_BGR2RGB);
    QImage image((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    image.bits();
    return image;
}

Mat Camera::QImage2Mat(QImage const& image)
{
    Mat tmp(image.height(),image.width(),CV_8UC3,(uchar*)image.bits(),image.bytesPerLine());
    Mat mat;
    cvtColor(tmp, mat,CV_BGR2RGB);
    return mat;
}

int Camera::getCameraCount()
{
    //获取可用相机数目
    if(cameracount>0)
        return cameracount;
    VideoCapture _capture;
    while(_capture.open(cameracount,CAP_DSHOW)){
        cameracount++;
        _capture.release();
    }
    return cameracount;

}
