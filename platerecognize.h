#ifndef PLATERECOGNIZE_H
#define PLATERECOGNIZE_H

#include <QObject>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>

//添加车牌识别训练模型
#include <opencv2/opencv.hpp>
#include "include/easypr.h"
using namespace easypr;
class PlateRecognize : public QObject
{
    Q_OBJECT
public:
    explicit PlateRecognize(QObject *parent = nullptr);

signals:
    void Recogned(Mat qPlateImg,QString plateStr);

public slots:
    void Recognize(Mat rgbImg);
private:
    //车牌识别
    easypr::CPlateRecognize m_plateRecognize;

};

#endif // PLATERECOGNIZE_H
