#include "pthreadpool.h"

PReserve::PReserve(QObject *parent): QObject(parent),QRunnable()
{
    setAutoDelete(true); //设置自动释放空间
}

void PReserve::recName(QSqlQuery q,QString name)
{
    p_name=name;
    m_q = q;
}

void PReserve::run()
{
    //qDebug()<<"进入子线程";


    m_q.prepare("SELECT P_now_count,P_all_count,P_reserve_count FROM parking WHERE P_name = :park_name;");
    m_q.bindValue(":park_name", p_name);
    m_q.exec();
    m_q.next();
    QString now_count = m_q.value(0).toString();
    QString all_count = m_q.value(1).toString();
    QString reserve_count = m_q.value(2).toString();
    //将停车场数据呈现到图表中

//    mysql_C.reserve = reserve_count.toInt();
//    mysql_C.parking_now_count = q.value(0).toInt();
    //qDebug()<<reserve_count.toInt()<<q.value(0).toInt();
    emit finish(reserve_count.toInt(),m_q.value(0).toInt()); //执行完毕

}


INIT::INIT(QObject *parent): QObject(parent)
{
}

void INIT::working(mysql mysql_c)
{

    mysql_c.create_car();  //初始化车库表格
    //建立车库预定表
    mysql_c.parking_reserve(); //建立车库预定表，并生成触发器
    //初始化停车表
    mysql_c.create_parking(); //创建车表

    mysql_c.Parking_init(); //初始化车表数据

    mysql_c.create_user(); //初始化用户表格
    qDebug()<<"子线程表格创建成功";
    emit finish();

}

//void CameraThread::run()
//{
//    camera = new QCamera(QCameraInfo::defaultCamera());

//    imageCapture = new QCameraImageCapture(camera);

//    emit cameraInitialized(camera,imageCapture); //通知主线程摄像头已经初始化
//}

Recognize::Recognize(QObject *parent) : QObject (parent)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
}

void Recognize::working(Mat rgbImg)
{
    //加载识别模型
    CPlateRecognize pr;
    pr.LoadSVM("E://parkingcar//Login//model//svm_hist.xml");

    pr.LoadANN("E://parkingcar//Login//model//ann.xml");

    pr.LoadChineseANN("E://parkingcar//Login//model//ann_chinese.xml");
    // new in v1.6
    pr.LoadGrayChANN("E://parkingcar//Login//model//annCh.xml");
    pr.LoadChineseMapping("E://parkingcar//Login//model//province_mapping");

    std::vector<easypr::CPlate> plates;
    m_plateRecognize.plateRecognize(rgbImg,plates,0);
    for (auto plate : plates) {
        std::cout << "plate: " << plate.getPlateStr() << std::endl;
    }
    qDebug()<<"内部输出";
    if(plates.size()>0)
    {
        //显示车牌图片和号码
        easypr::CPlate plate = plates[0];
        Mat plateImg = plate.getPlateMat();
        QImage qPlateImg(plateImg.data,plateImg.cols,plateImg.rows,static_cast<int>(plateImg.step), QImage::Format_RGB888);
        //ui->screen_label->setPixmap(QPixmap::fromImage(qPlateImg));

        QString plateStr = QString::fromLocal8Bit(plate.getPlateStr().c_str());
        // 获取车牌号码的部分字符串
        QStringList list = plateStr.split(":");
        if (list.size() >= 2) {
            plateStr = list[1].trimmed();
        }

        //将platestr返回出去,车牌照片和车牌号
        emit finish(qPlateImg,plateStr);
    }
}
