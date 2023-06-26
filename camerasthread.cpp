#include "camerasthread.h"
Camera::Camera(int number)
{
    //摄像头初始化
    //输出所有摄像头信息
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if(cameras.count() > 0)
    {
        foreach (const QCameraInfo &cameraInfo, cameras)
            qDebug() << cameraInfo.deviceName();
        //CameraThread* thread = new CameraThread(this);
        //现在主线程传递处理的信号
       camera = new QCamera(cameras.at(number));  //编号为第几个摄像头
    }
    //抓取图片
    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->start();
    //TODO将camera图像传给主线程
    //用户摄像头截图
    imageCapture = new QCameraImageCapture(camera);
    camera_statue = true;

}

Camera::~Camera()
{
    //释放资源
    camera->stop();
    delete imageCapture;
    delete camera;
}


int Camera::getCameraCount()
{
    //获取可用相机数目
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    return cameras.count();

}

void Camera::CameraShow()
{
    //信号槽祖册
    /*QCameraImageCapture::imageCaptured(int id, const QImage &preview)
      Signal emitted when the frame with request id was captured, but not processed and saved yet. Frame preview can be displayed to user.
     *
     * */
    connect(imageCapture,&QCameraImageCapture::imageCaptured,this,[=](int id,const QImage &preview)
    {
        qDebug()<<"图片传出";
        emit Camera_Image_signal(id,preview);

    });
    //进入图片子线程
    qDebug()<<"进入图片子线程";
    while(camera_statue)
    {

        if(camera->status() == QCamera::ActiveStatus)
        {
            //抓取图片
            qDebug()<<"开始抓取图片";
            imageCapture->capture();
            //使用信号槽
        }
        QThread::msleep(1000); //线程休眠100毫秒
    }

}
