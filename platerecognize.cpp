#include "platerecognize.h"
#include <QDebug>
PlateRecognize::PlateRecognize(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Mat>("Mat");

    //加载车牌识别的训练模型
    // 1. 加载车牌识别模型 -不加载，识别准确率超低
    //车牌识别部分，加入线程
    CPlateRecognize pr;
    pr.LoadSVM("E://parkingcar//Parking-system//model//svm_hist.xml");

    pr.LoadANN("E://parkingcar//Parking-system//model//ann.xml");

    pr.LoadChineseANN("E://parkingcar//Parking-system//model//ann_chinese.xml");
    // new in v1.6
    pr.LoadGrayChANN("E://parkingcar//Parking-system//model//annCh.xml");
    pr.LoadChineseMapping("E://parkingcar//Parking-system//model//province_mapping");
}



// 传入参数rgbImg 图片 Mat rgbImg;
// 传出图片QImage qPlateImg，传出 QString plateStr

void PlateRecognize::Recognize(Mat rgbImg)
{
     Mat localImg = rgbImg.clone(); // 复制 Mat 对象到局部变量
    //使用easyPR获取车牌信息
    std::vector<easypr::CPlate> plates;
    m_plateRecognize.plateRecognize(localImg,plates,0);
    for (auto plate : plates) {
        std::cout << "plate: " << plate.getPlateStr() << std::endl;
    }
    qDebug()<<"内部子线程输出,线程ID:"<<pthread_self();
    if(plates.size()>0)
    {
        //显示车牌图片和号码
        easypr::CPlate plate = plates[0];
        Mat plateImg = plate.getPlateMat();
        //QImage qPlateImg(plateImg.data,plateImg.cols,plateImg.rows,static_cast<int>(plateImg.step), QImage::Format_RGB888);
        //传出图片
//        ui->screen_label->setPixmap(QPixmap::fromImage(qPlateImg));

        QString plateStr = QString::fromLocal8Bit(plate.getPlateStr().c_str());
        // 获取车牌号码的部分字符串
        QStringList list = plateStr.split(":");
        if (list.size() >= 2) {
            plateStr = list[1].trimmed();
        }
        // 传出
//        ui->Car_idinput->setText(plateStr);
//        ui->Car_output->setText(plateStr);
        qDebug()<<plateStr;
        //std::cout << "plate: " << plate.getPlateStr() << std::endl;
        emit Recogned(plateImg,plateStr); //传出图片和识别的字符串
    }

}
