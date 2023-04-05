#include "car.h"
#include "ui_car.h"
#include <mainwindow.h> //引用登录函数的初始化数据库
Car::Car(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Car)
{
    ui->setupUi(this);
    setWindowTitle("停车场管理系统车牌识别");
    mysql(); //mysql初始化
    // 隐藏行号
    ui->tablectrol->verticalHeader()->setVisible(false);

    //先给视频一个图片
    // 创建QPixmap对象，加载png图像存储到pix变量中，使用new关键字分配堆内存来储存该对象
    QPixmap *pix = new QPixmap(":/images/car.jpg");
    QSize sz = ui->label_video->size();
    ui->label_video->setPixmap(pix->scaled(sz));

    // 设置默认显示第二页
    ui->stack->setCurrentIndex(0);
    //链接
    connect(ui->MainButton,&QPushButton::clicked,this,&Car::SwitchPage);
    connect(ui->messageButton,&QPushButton::clicked,this,&Car::SwitchPage);
    connect(ui->CtrolButton,&QPushButton::clicked,this,&Car::SwitchPage);
}

Car::~Car()
{
    mysql().mysql_close();
    delete ui;
}
void Car::SwitchPage(){
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button==ui->MainButton)
        ui->stack->setCurrentIndex(0);
    if(button==ui->messageButton)
        ui->stack->setCurrentIndex(1);
    if(button==ui->CtrolButton)
        ui->stack->setCurrentIndex(2);

}

//当点击用户控制页面的时候，
//初始化数据，如果连接成功然后将将数据库内部的用户信息呈现到tableCtrol内部

void Car::on_CtrolButton_clicked()
{
    //初始化数据连接
    ui->tablectrol->size();//设置表格

    //设置表头
    QStringList header;
    header<<"ID"<<"账户"<<"密码"<<"手机号"<<"姓名";

    ui->tablectrol->setHorizontalHeaderLabels(header);

    //初始化表
//    ui->tablectrol->setItem(0,0,new QTableWidgetItem(""));
//    ui->tablectrol->setItem(1,0,new QTableWidgetItem(""));
//    ui->tablectrol->setItem(2,0,new QTableWidgetItem(""));
//    ui->tablectrol->setItem(3,0,new QTableWidgetItem(""));
//    ui->tablectrol->setItem(4,0,new QTableWidgetItem(""));

//    ui->tablectrol->setItem(0,1,new QTableWidgetItem(""));
//    ui->tablectrol->setItem(1,1,new QTableWidgetItem(""));
//    ui->tablectrol->setItem(2,1,new QTableWidgetItem(""));
//    ui->tablectrol->setItem(3,0,new QTableWidgetItem(""));
//    ui->tablectrol->setItem(4,0,new QTableWidgetItem(""));

    //使用MySQL语句查询
    QString sqlstr="select id,username,password,telephone,truename  from USER;";
    QSqlQuery q;
    q.prepare(sqlstr);
    int i = 0;
    if(q.exec())
    {
        while (q.next()) {
            //设置表格行数，每一次加一行
            ui->tablectrol->setRowCount(i+1);
            ui->tablectrol->setItem(i,0,new QTableWidgetItem(q.value(0).toString()));
            ui->tablectrol->item(i,0)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

            ui->tablectrol->setItem(i,1,new QTableWidgetItem(q.value(1).toString()));
            ui->tablectrol->item(i,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

            ui->tablectrol->setItem(i,2,new QTableWidgetItem(q.value(2).toString()));
            ui->tablectrol->item(i,2)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

            ui->tablectrol->setItem(i,3,new QTableWidgetItem(q.value(3).toString()));
            ui->tablectrol->item(i,3)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

            ui->tablectrol->setItem(i,4,new QTableWidgetItem(q.value(4).toString()));
            ui->tablectrol->item(i,4)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            i++;
        }
    }



}
