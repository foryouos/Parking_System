#ifndef CAR_H
#define CAR_H

#include <QWidget>
#include "mysql.h"
#include <QDateTime>


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

private:
    Ui::Car *ui;
};

#endif // CAR_H
