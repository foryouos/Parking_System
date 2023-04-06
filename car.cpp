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
    mysql().create_car();  //初始化车库表格
    //初始化停车表
    mysql().create_parking(); //创建车表


    //判断是否在


    // 隐藏行号
    //ui->tablectrol->verticalHeader()->setVisible(false);

    //设置管理员列表第一列ID为只读

    //创建饼图
    //create_pie();




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
            //第一列
            ui->tablectrol->setItem(i,0,new QTableWidgetItem(q.value(0).toString()));
            ui->tablectrol->item(i,0)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

            //为添加过后的每i行第0列设置数据只读
            ui->tablectrol->item(i,0)->setFlags(ui->tablectrol->item(i,0)->flags()^Qt::ItemIsEditable);

            //第二列
            ui->tablectrol->setItem(i,1,new QTableWidgetItem(q.value(1).toString()));
            ui->tablectrol->item(i,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            //第三列
            ui->tablectrol->setItem(i,2,new QTableWidgetItem(q.value(2).toString()));
            ui->tablectrol->item(i,2)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            //第四列
            ui->tablectrol->setItem(i,3,new QTableWidgetItem(q.value(3).toString()));
            ui->tablectrol->item(i,3)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            //第五列
            ui->tablectrol->setItem(i,4,new QTableWidgetItem(q.value(4).toString()));
            ui->tablectrol->item(i,4)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            i++;
        }
        //在底部额外添加一行
        ui->tablectrol->setRowCount(i+1);

    }



}
//获取当前用户鼠标太Table WIdget所在的行
//并获取用户在哪一行做出修改，然后根据用户ID更新用户信息
void Car::on_ButtonModify_clicked()
{
    //读取当前鼠标所在的行
    int row = ui->tablectrol->currentRow();
    qDebug()<<row;
    if(row>=0)
    {
        //读取行对应的数据
        QString ID = ui->tablectrol->item(row,0)->text(); //ID为只读，作为唯一标识符
        //读取其它信息
        QString username = ui->tablectrol->item(row,1)->text();
        QString password = ui->tablectrol->item(row,2)->text();
        QString telephone = ui->tablectrol->item(row,3)->text();
        QString truename = ui->tablectrol->item(row,4)->text();
        //输出信息测试
        //qDebug()<<ID<<username<<password<<telephone<<truename;

        //将读取到的数据更新到数据库
        QString encryptedPassword = encryptPassword(password); // 对密码进行加密
        QString sql_update = QStringLiteral("UPDATE USER SET username='%1', password='%2', telephone='%3',truename='%4' WHERE id='%5';").arg(username,encryptedPassword,telephone,truename,ID);
        QSqlQuery query;
        query.prepare(sql_update);
        if (query.exec()) {
            if (query.numRowsAffected() == 1) {
                qDebug() << "User updated successfully.";
                Car::on_CtrolButton_clicked(); //模拟点击，更新
            } else {
                qDebug() << "Could not update user with ID:" << ID;
            }
        } else {
            //检查执行语句
            qDebug()<<sql_update;
            qDebug() << "Error executing SQL statement:" ;
        }
    }

}

void Car::on_ButtonADD_clicked()
{
    //读取当前鼠标所在的行
    int row = ui->tablectrol->currentRow();
    qDebug()<<row;
    if(row>=0)
    {
        //读取用户添加数据后鼠标所在的行
        QString ID = ui->tablectrol->item(row,0)->text();
        QString username = ui->tablectrol->item(row,1)->text();
        QString password = ui->tablectrol->item(row,2)->text();
        QString telephone = ui->tablectrol->item(row,3)->text();
        QString truename = ui->tablectrol->item(row,4)->text();
        //输出信息测试
        //qDebug()<<ID<<username<<password<<telephone<<truename;

        //将读取到的数据更新到数据库
        QString encryptedPassword = encryptPassword(password); // 对密码进行加密
        QString sql_insert = QStringLiteral("insert into USER(id,username,password,telephone,truename) values('%1','%2','%3','%4','%5');").arg(ID,username, encryptedPassword,telephone,truename);

        if (mysql().execute_bool(sql_insert))
        {
            qDebug() << "User Add successfully.";
            //为添加过后的每i行第0列设置数据只读
            ui->tablectrol->item(row,0)->setFlags(ui->tablectrol->item(row,0)->flags()^Qt::ItemIsEditable);
            Car::on_CtrolButton_clicked(); //模拟点击，更新
        }
        else
        {
            qDebug()<<sql_insert;
            qDebug() << "User Add error" << ID;
        }


    }
}

void Car::on_ButtonDelete_clicked()
{
    //读取当前鼠标所在的行
    int row = ui->tablectrol->currentRow();
    qDebug()<<row;
    if(row>=0)
    {
        //读取用户添加数据后鼠标所在的行
        QString ID = ui->tablectrol->item(row,0)->text();

        QString sql_Drop = QStringLiteral("DELETE FROM USER WHERE id = '%1';").arg(ID);

        if (mysql().execute_bool(sql_Drop))
        {
            qDebug() << "User Add successfully.";
            //恢复可读
            ui->tablectrol->item(row,0)->setFlags(ui->tablectrol->item(row,0)->flags() | Qt::ItemIsEditable);
            Car::on_CtrolButton_clicked(); //模拟点击，更新
        }
        else
        {
            qDebug()<<sql_Drop;
            qDebug() << "User Add error" << ID;
        }


    }
}
//车辆入库将信息提交到数据库
void Car::on_submitCar_clicked()
{

    //读取车牌号
    QString license_plate = ui->Car_idinput->text(); //读取输入或识别出来的车牌号
    //获取当前的时间
    // 获取当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    // 将时间转换为字符串格式（格式化为 "yyyy-MM-dd hh:mm:ss"）存储到MySQL的database
    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    //将信息上传到数据库
    //位置
    QString location = "新科停车场";

    QString sql_submitCar = QStringLiteral("INSERT INTO CAR (license_plate, check_in_time,location) VALUES ('%1','%2','%3');").arg(license_plate,formattedDateTime,location);

    if(mysql().execute_bool(sql_submitCar))
    {
        qDebug()<<"车牌数据插入成功";
        ui->Car_idinput->clear();  //清空输入框
        QMessageBox::information(this,"停车入库","车牌入库成功!");
    }
    else {
        qDebug()<<"车牌插入失败";
        qDebug()<<sql_submitCar;
        QMessageBox::information(this,"停车入库","车牌入库失败！！");

    }


}

void Car::on_messageButton_clicked()
{
    // 隐藏车辆信息的列号
    ui->tableCar->verticalHeader()->setVisible(false);
    // 设置表头
    QStringList header;
    header<<"ID"<<"车牌号"<<"入库时间"<<"出库时间"<<"费用"<<"停车地点";
    ui->tableCar->setHorizontalHeaderLabels(header);
    //让列号 自适应列表大小
    //ui->tableCar->resizeColumnsToContents(); //自适
    //设置列号的宽度
    ui->tableCar->setColumnWidth(2, 200);
    ui->tableCar->setColumnWidth(3, 200);
    //读取数据库的数据

    //使用MySQL语句查询车库信息，根据时间倒序
    QString sqlstrCar="select id,license_plate,check_in_time,check_out_time,fee,location from CAR  ORDER BY check_in_time DESC;";
    QSqlQuery q;
    q.prepare(sqlstrCar);
    int i = 0;
    if(q.exec())
    {

        while (q.next()) {
            //设置表格行数，每一次加一行
            ui->tableCar->setRowCount(i+1);
            //第一列
            ui->tableCar->setItem(i,0,new QTableWidgetItem(q.value(0).toString()));
            ui->tableCar->item(i,0)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

            //为添加过后的每i行第0列设置数据只读
            //ui->tablectrol->item(i,0)->setFlags(ui->tablectrol->item(i,0)->flags()^Qt::ItemIsEditable);

            //第二列
            ui->tableCar->setItem(i,1,new QTableWidgetItem(q.value(1).toString()));
            ui->tableCar->item(i,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            //第三列
            ui->tableCar->setItem(i,2,new QTableWidgetItem(q.value(2).toString()));
            ui->tableCar->item(i,2)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            //第四列
            ui->tableCar->setItem(i,3,new QTableWidgetItem(q.value(3).toString()));
            ui->tableCar->item(i,3)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            //第五列
            ui->tableCar->setItem(i,4,new QTableWidgetItem(q.value(4).toString()));
            ui->tableCar->item(i,4)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            //第六列
            ui->tableCar->setItem(i,5,new QTableWidgetItem(q.value(5).toString()));
            ui->tableCar->item(i,5)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            i++;
        }
    }
}
//创建饼图

