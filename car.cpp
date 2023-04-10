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

    mysql().Parking_init(); //初始化车表数据

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
    //检查车牌是否合规，不合规直接退出
//    if(!checkPlateNumber(license_plate))
//    {
//        qDebug()<<license_plate ;
//        QMessageBox::information(this,"识别失败","您输入的车牌号不合规");
//        return;
//    }



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
        //车牌插入成功后，更新车库数据
        mysql().parking_acc(); //让现有车库加一

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
    header<<"ID"<<"车牌号"<<"入库时间"<<"出库时间"<<"费用"<<"停车地点"<<"车费单价";
    ui->tableCar->setHorizontalHeaderLabels(header);
    //让列号 自适应列表大小
    //ui->tableCar->resizeColumnsToContents(); //自适
    //设置列号的宽度
    ui->tableCar->setColumnWidth(2, 200);
    ui->tableCar->setColumnWidth(3, 200);
    //读取数据库的数据

    //使用MySQL语句查询车库信息，根据时间倒序
    QString sqlstrCar="SELECT id, license_plate, check_in_time, check_out_time, fee, location, P_fee FROM CAR JOIN parking ON Car.location = parking.P_name ORDER BY check_in_time DESC;";
    QSqlQuery q;
    q.prepare(sqlstrCar);
    int i = 0;
    if(q.exec())
    {

        while (q.next()) {
            print_widget(q,i);
            i++;
        }
    }
}
//创建饼图
//收费函数
int Car::fee_charge(QDateTime oldDateTime, QDateTime currentDateTime,QString license_plate)
{
    //计算彼此的秒差
    qint64 seconds = oldDateTime.secsTo(currentDateTime);
    long long hours = std::abs(seconds) / 3600LL;
    qDebug()<<seconds << hours;

    //计算出费用
    if(hours<0.5) //如果小于半个小时，则不收费
    {
        return 0;
    }
    QString sql_fee= QStringLiteral("SELECT parking.P_fee FROM parking JOIN car ON car.location = parking.P_name WHERE CAR.license_plate='%1';").arg(license_plate);
    QSqlQuery q = mysql().execute(sql_fee);
    q.next();
    int p_fee = q.value(0).toInt();

    int fee = int(hours) * p_fee;

    //首先根据车牌号，
    return fee;
}
//输出widget列表数据
void Car::print_widget(QSqlQuery q,int i)
{
    //设置表格行数，每一次加一行
    ui->tableCar->setRowCount(i+1);
     //第一列
    ui->tableCar->setItem(i,0,new QTableWidgetItem(q.value(0).toString()));
    ui->tableCar->item(i,0)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    //为添加过后的每i行第0列设置数据只读
    ui->tableCar->item(i,0)->setFlags(ui->tableCar->item(i,0)->flags()^Qt::ItemIsEditable);

    //第二列
    ui->tableCar->setItem(i,1,new QTableWidgetItem(q.value(1).toString()));
    ui->tableCar->item(i,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    //第三列
    //使用QDateTimeEdit
    QDateTimeEdit *editDateTime = new QDateTimeEdit();
    editDateTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    editDateTime->setDateTime(q.value(2).toDateTime());

    ui->tableCar->setCellWidget (i, 2, editDateTime);

    //ui->tableCar->setItem(i,2,new QTableWidgetItem(q.value(2).toString()));
    //ui->tableCar->item(i,2)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    //第四列

    //如果数据库输出时间为空
    if (q.value(3).isNull())
    {
        ui->tableCar->setItem(i,3,new QTableWidgetItem(q.value(3).toString()));
        ui->tableCar->item(i,3)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    }
    else
    {
        QDateTimeEdit *editDateTime2 = new QDateTimeEdit();
        editDateTime2->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
        editDateTime2->setDateTime(q.value(3).toDateTime());
        ui->tableCar->setCellWidget(i, 3, editDateTime2);
    }
    //第五列
    ui->tableCar->setItem(i,4,new QTableWidgetItem(q.value(4).toString()));
    ui->tableCar->item(i,4)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    //第六列
    ui->tableCar->setItem(i,5,new QTableWidgetItem(q.value(5).toString()));
    ui->tableCar->item(i,5)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    //第七列
    ui->tableCar->setItem(i,6,new QTableWidgetItem(q.value(6).toString()));
    ui->tableCar->item(i,6)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
}
//当出库操作之后
void Car::on_DeleteCar_clicked()
{
    //读取车牌号
    QString license_plate = ui->Car_output->text(); //读取输入或识别出来的车牌号
//    if(!checkPlateNumber(license_plate))
//    {
//        QMessageBox::information(this,"识别失败","您输入的车牌号不合规");
//        return;
//    }
    //先检查车牌是否已入库，若未入库则报错，并返回时间
    QString sqlplate_check= QStringLiteral("select id from CAR where license_plate = '%1' AND check_out_time IS NULL; ").arg(license_plate);
    QSqlQuery query =mysql().execute(sqlplate_check);
    if(!query.size()) //如果不存在此数据
    {
         QMessageBox::information(this,"入库检测","当前车牌不在车库当中!!");
         return ;
    }
    //获取当前车牌号的时间
    QString sql_time= QStringLiteral("select check_in_time from CAR where license_plate = '%1' ORDER BY check_in_time DESC;").arg(license_plate);
    QSqlQuery q = mysql().execute(sql_time);
    q.next();
    QDateTime oldDateTime = q.value(0).toDateTime();



    //获取当前的时间
    // 获取当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    // 将时间转换为字符串格式（格式化为 "yyyy-MM-dd hh:mm:ss"）存储到MySQL的database
    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    //qDebug()<<oldDateTime<<currentDateTime;
    //根据时间计算费用
    //调用时间函数，传入参数，识别的车牌号（去求过去的时间)，还有现在的时间,根据时间差去求所需要的费用
    qDebug()<<oldDateTime<<currentDateTime;
    int fee = fee_charge(oldDateTime,currentDateTime,license_plate);


    //将信息上传到数据库
    qDebug()<<formattedDateTime<<fee<<license_plate;


    //QString sql_submitfee = QStringLiteral("UPDATE CAR SET check_out_time='%1', fee='%2' WHERE license_plate='%3';").arg(formattedDateTime,fee,QChar::fromLatin1(license_plate.toLatin1()[0]));
    QSqlQuery qupdate;
    qupdate.prepare("UPDATE CAR SET check_out_time=:check_out_time, fee=:fee WHERE license_plate=:license_plate");
    qupdate.bindValue(":check_out_time", formattedDateTime);
    qupdate.bindValue(":fee", fee);
    qupdate.bindValue(":license_plate", license_plate);

    if(qupdate.exec())
    {
        qDebug()<<"车辆出库成功";
        ui->Car_output->clear();  //清空输入框
        QString message = QString("出库成功，车牌号'%1'需要支付'%2'元").arg(license_plate, QString::number(fee, 'f', 2));

        QMessageBox::information(this,"停车出库库",message);
        //车牌插入成功后，更新车库数据
        mysql().parking_dec(); //让现有车位数量减一

    }
    else {
        qDebug()<<"车牌出库失败";
        //qDebug()<<sql_submitfee;
        QMessageBox::information(this,"停车入库","车牌入库失败！！");

    }
}


//点击查询后，根据用户输入的时间和车牌号，来进行输出数据
void Car::on_Carcheck_clicked()
{
    //读取用户的输入数据
    QString Car_name = ui->find_lineEdit->text();
    QDateTime begin_datetimes = ui->begin_dateTimeEdit->dateTime();
    QDateTime end_datetimes = ui->end_dateTimeEdit->dateTime();
    QString begin_time = begin_datetimes.toString("yyyy-MM-dd hh:mm:ss");
    QString end_time = end_datetimes.toString("yyyy-MM-dd hh:mm:ss");
    //书写sql语句
    //如果用户没有输入车牌号 则不显示
    if(Car_name.isEmpty())
    {
        //名字为空根据时间查询
        //qDebug()<<"名字为空"<<begin_time<<end_time;
        QSqlQuery q;
        q.prepare("SELECT id, license_plate, check_in_time, check_out_time, fee, location, P_fee from car JOIN parking ON car.location = parking.P_name WHERE check_in_time BETWEEN :begin_time AND :end_time;");
        q.bindValue(":begin_time", begin_time);
        q.bindValue(":end_time", end_time);
        int i = 0;
        if(q.exec())
        {

            while (q.next()) {
                print_widget(q,i);
                i++;
            }
        }
     }
    else
    {   //名字不为空，根据时间和 名字
            //qDebug()<<"名字为空"<<begin_time<<end_time;
            QSqlQuery q;
            q.prepare("SELECT id, license_plate, check_in_time, check_out_time, fee, location, P_fee from car JOIN parking ON car.location = parking.P_name WHERE check_in_time BETWEEN :begin_time AND :end_time AND license_plate =:Car_name;");
            q.bindValue(":begin_time", begin_time);
            q.bindValue(":end_time", end_time);
            q.bindValue(":Car_name",Car_name);
            int i = 0;
            if(q.exec())
            {
                while (q.next())
                {
                    print_widget(q,i);
                    i++;
                }
            }
    }

    //调用函数输出


}
//读取当前鼠标所在车牌号，进行删除
void Car::on_Car_delete_clicked()
{
    //读取当前鼠标所在车辆信息表的行数
    int row = ui->tableCar->currentRow();
    qDebug()<<row;
    if(row>=0)
    {
        //读取用户点击位置的车牌号
        QString license_plate = ui->tableCar->item(row,1)->text();

        QString sql_Drop_car = QStringLiteral("DELETE FROM CAR WHERE license_plate = '%1';").arg(license_plate);

        if (mysql().execute_bool(sql_Drop_car))
        {
            qDebug() << "CAR delete successfully.";
            Car::on_messageButton_clicked(); //模拟点击，更新
        }
        else
        {
            qDebug()<<sql_Drop_car;
            qDebug() << "User Add error" << license_plate;
        }


    }
}
