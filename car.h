#ifndef CAR_H
#define CAR_H

#include <QWidget>
#include "mysql.h"
#include <QDateTime>
#include <QRegExp>
#include <QRegularExpression>
#include <QDateTimeEdit>
#include <QTimer>
#include <QMediaPlayer>
#include <QVideoWidget>

#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QFileDialog>
#include <QDateTime>
#include <QtGui/QScreen>

namespace Ui {
class Car;
}

class Car : public QWidget
{
    Q_OBJECT

public:
    explicit Car(QWidget *parent = nullptr);
    ~Car();
    //切换函数
    void SwitchPage();
    //检查输入的车牌号是否合规
    bool checkPlateNumber(QString);
    //时间计费函数
    int fee_charge(QDateTime,QDateTime,QString);
    //设置函数输出到Qtable widget中数据，传入参数数据库对象
    void print_widget(QSqlQuery q,int i);

private slots:

    //初始化停车场数据
    //初始化停车场数据

    //点击用户管理页面
    void on_CtrolButton_clicked();
    //点击修改用户信息
    void on_ButtonModify_clicked();

    void on_ButtonADD_clicked();

    void on_ButtonDelete_clicked();

    void on_submitCar_clicked();

    void on_messageButton_clicked();

    //void on_MainButton_clicked();

    //创建饼图
    //void create_pie();

    //创建车位展示
    void park_num();

    void on_DeleteCar_clicked();





    void on_Carcheck_clicked();

    void on_Car_delete_clicked();


    //摄像头初始化
    void camera_Init();
    //打开视频文件初始化
    void video_Init();



    void on_fileopen_clicked();

    void on_video_start_clicked();

    void on_video_pause_clicked();


    void Get_Duration();
    void Slider_Changed();
    void On_Position_Changed(qint64 position);

    void on_camera_take_clicked();

    void on_camera_button_clicked();

private:
    Ui::Car *ui;
    //定义mysql类
    mysql mysql_C;
    //数据库执行类
    QSqlQuery q;


    //音频文件对象
    QMediaPlayer *player;
    //视频文件对象
    QVideoWidget *videowidget;
    QString fileName;
    int pos;
    //视频帧截屏
    QPixmap pixmap;


    //创建摄像头对象
    QCamera *camera;
    //显示摄像头的指针
    QCameraViewfinder *viemfinder;
    //捕获图片,操作用于捕获的对象
    QCameraImageCapture *imageCapture;

    //判断视频是否暂停
    bool cameraPaused = false;
    QWidget *centerWidget;

};

#endif // CAR_H
