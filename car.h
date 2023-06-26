#ifndef CAR_H
#define CAR_H

#include <QWidget>
#include "mysql.h"
#include <QtSql> //检测数据库
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

// 添加下面这行头文件包含语句
#include <QChart>
#include <QPieSeries>
#include <QChartView>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>

//添加线程池
#include <QThreadPool>
#include "pthread.h"

#include "camerasthread.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/types_c.h"
#include "platerecognize.h"
#include "updatapie.h"
#include "camerasthread.h"
#include <QGraphicsColorizeEffect>

#include "pthreadpool.h"
using namespace cv;
//添头文件
QT_CHARTS_USE_NAMESPACE

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

signals:
    void starting(QSqlQuery q,QString name);
    void Camerathread_open_Signale();  //运行摄像头信号
    void Plate_start(Mat rgbImg);
    void Plate_Recognize(Mat rgbImg,QString plateStr); //通知EasyPR识别图片
    // 更新饼图
    void UpPieData(QString m_Parking_name,int m_reserve,int m_now_count_L);
    // 现有车辆-1/添加+1 调用 MySQl信号，对数据进行修改
    void now_count_dec_signal(); // 累减
    void now_count_acc_signal(); // 累加
    // 到线程池中上传车牌信息
    void sendPlate_signal(mysql* mysql,QString license_plate,QString formattedDateTime,QString location);


private slots:

    //槽函数
    void on_PieSliceHighlight(bool show);

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

    void on_MainButton_clicked();


    void updateImage(QImage);


    void on_check_camera_clicked();

//    void on_thread_camera_init_clicked();

//    void on_thread_release_clicked();

//    void on_camera_open_button_clicked();

protected:
    //当窗口大小发生变化调用此函数，重写此函数
    //当窗口大小发生变化时，调整QWidget的大小
    void resizeEvent(QResizeEvent* ev) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event)  Q_DECL_OVERRIDE;

private:
    Ui::Car *ui;
    //定义mysql类
    mysql *mysql_C;
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
    //饼图
    QChart *qchart;
    QPieSeries *series;
    QChartView *chartView;
    //获取分块
    QPieSlice *slice;  //饼图分块
    void PIE();
    //车牌识别
    easypr::CPlateRecognize m_plateRecognize;

    //创建饼图
    void create_pie(int reserve);
    //计时器
    QTimer* timer = new QTimer(this);

    //监控处多线程
    //显示摄像头的指针
    QCameraViewfinder *threadviemfinder;
};

#endif // CAR_H
