#include "car.h"
#include "ui_car.h"

Car::Car(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Car)
{
    ui->setupUi(this);
    setWindowTitle("停车场管理系统车牌识别");



}

Car::~Car()
{
    delete ui;
}
