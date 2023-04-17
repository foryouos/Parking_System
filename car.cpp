#include "car.h"
#include "ui_car.h"
#include <mainwindow.h> //引用登录函数的初始化数据库
#include "QFileDialog"
#include <QDebug>
Car::Car(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Car)
{
    ui->setupUi(this);
    setWindowTitle("停车场管理系统车牌识别");
    //车表初始化，更新几个数据

    //播放视频初始化
    video_Init();
    //调用摄像头初始化
    camera_Init();
    //默认使用摄像头工作
    viemfinder->setVisible(true); //显示摄像头显示区域
    camera->start();  //让摄像头开始工作
    //创建饼图
    //实现简单的数据呈现
    park_num();
    //create_pie();

    //先给视频一个图片
    // 创建QPixmap对象，加载png图像存储到pix变量中，使用new关键字分配堆内存来储存该对象

    // 设置默认显示第二页
    ui->stack->setCurrentIndex(0);
    //链接
    connect(ui->MainButton,&QPushButton::clicked,this,&Car::SwitchPage);
    connect(ui->messageButton,&QPushButton::clicked,this,&Car::SwitchPage);
    connect(ui->CtrolButton,&QPushButton::clicked,this,&Car::SwitchPage);

    //定时更新
//    QSqlTableModel model(nullptr, mysql_C.get_db());
//    model.setTable("reservations");
//    model.select();
//    // 监视模型以检测更改
//    while (true) {
//        if (model.rowCount() != model.select()) {
//            qDebug() << "表已更新";
//            // 在此进行处理更新操作
//            park_num();
//        }
//    }
    QTimer* timer = new QTimer(this); // 创建定时器对象
    connect(timer, &QTimer::timeout, this, &Car::checkMySQLData); // 将timeout信号连接到槽函数
    timer->start(1000); // 启动定时器，每隔1秒检查一次

    //加载车牌识别的训练模型
    // 1. 加载车牌识别模型
    CPlateRecognize pr;
    pr.LoadSVM("E://Cstudy//QT//QT_exercise//easyPR//easypr2//model//svm_hist.xml");

    pr.LoadANN("E://Cstudy//QT//QT_exercise//easyPR//easypr2//model//ann.xml");

    pr.LoadChineseANN("E://Cstudy//QT//QT_exercise//easyPR//easypr2//model//ann_chinese.xml");


    // new in v1.6
    pr.LoadGrayChANN("E://Cstudy//QT//QT_exercise//easyPR//easypr2//model//annCh.xml");
    pr.LoadChineseMapping("E://Cstudy//QT//QT_exercise//easyPR//easypr2//model//province_mapping");

}

Car::~Car()
{
    mysql_C.mysql_close();
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
// 判断车牌是非合法

bool Car::checkPlateNumber(QString license_plate)
{
    //判断车牌位数企微，读取QString的位数
    if(license_plate.length() != 7&&license_plate.length() != 8)
    {
        QMessageBox::information(this,"入场失败","车牌号位数不对");
        ui->Car_idinput->clear();
        return false; //不满足7位，非法
    }
    //读取车牌的第一位，是否在所有省份的简称里面
    QString province = "京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼"; //省份简称
    if(!province.contains(license_plate.left(1)))
    { //判断首位是否省份简称
        ui->Car_idinput->clear();
        QMessageBox::information(this,"入场失败","首字母不含省份");
        return false;
    }
    //序列号共5位，包括英文和数字，其中第一位为英文字母，表示该车的使用性质；第二位为字母或数字，表示该车的行驶区域；后面三位为数字，表示该车的序号；
    QString letter = "ABCDEFGHJKLMNPQRSTUVWXYZ"; //序列号中的英文字母
    QString digit = "0123456789"; //序列号中的数字
    if(!letter.contains(license_plate.mid(1,1)))
    {
        QMessageBox::information(this,"入场失败","第二位不符合规定");
        ui->Car_idinput->clear();
        return false; //判断第二位是否符合规定
    }

    return true;
}



//当点击用户控制页面的时候，
//初始化数据，如果连接成功然后将将数据库内部的用户信息呈现到tableCtrol内部

void Car::on_CtrolButton_clicked()
{
    //判断摄像头和视频，如果视频暂停，隐藏，如果摄像头，隐藏即可
    if(camera->state() == QCamera::ActiveState||camera->state() == QCamera::LoadedState||player->state() == QMediaPlayer::PlayingState||player->state() == QMediaPlayer::PausedState)
    {
        player->stop(); //停止播放文件
        videowidget->setVisible(false); //隐藏文件播放的QVideoWidget
        viemfinder->setVisible(false); //隐藏摄像头显示区域
    }
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
        if(telephone.length()>11)
        {
            QMessageBox::information(this,"添加失败","您输入的手机号不符合11位");
            return;
        }
        QString truename = ui->tablectrol->item(row,4)->text();
        //输出信息测试
        //qDebug()<<ID<<username<<password<<telephone<<truename;

        //将读取到的数据更新到数据库
        QString encryptedPassword = encryptPassword(password); // 对密码进行加密
        QString sql_update = QStringLiteral("UPDATE USER SET username='%1', password='%2', telephone='%3',truename='%4' WHERE id='%5';").arg(username,encryptedPassword,telephone,truename,ID);

        q.prepare(sql_update);
        if (q.exec()) {
            if (q.numRowsAffected() == 1) {
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
        if(telephone.length()>11)
        {
            QMessageBox::information(this,"添加失败","您输入的手机号不符合11位");
            return ;
        }
        QString truename = ui->tablectrol->item(row,4)->text();
        //输出信息测试
        //qDebug()<<ID<<username<<password<<telephone<<truename;

        //将读取到的数据更新到数据库
        QString encryptedPassword = encryptPassword(password); // 对密码进行加密
        QString sql_insert = QStringLiteral("insert into USER(id,username,password,telephone,truename) values('%1','%2','%3','%4','%5');").arg(ID,username, encryptedPassword,telephone,truename);

        if (mysql_C.execute_bool(sql_insert))
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

        if (mysql_C.execute_bool(sql_Drop))
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
    if(!checkPlateNumber(license_plate))
    {
        qDebug()<<license_plate ;
        //QMessageBox::information(this,"识别失败","您输入的车牌号不合规");
        return;
    }


    qDebug()<<"输入时:"<<mysql_C.reserve;
    if((mysql_C.parking_now_count+mysql_C.reserve)>mysql_C.parking_count)
    {
        //当预定车位满时，判断车牌是否在预定表中，如果在则入库，若不在则出库

        qDebug()<<"现有车位"<<mysql_C.parking_now_count<<"预约量:"<<mysql_C.reserve<<"总车位:"<<mysql_C.parking_count;
        QMessageBox::information(this,"入场失败","当前停车场已满！");
        return ;
    }
    //检查当前车牌是否在车库内部还未出库

    q.prepare("select check_in_time,check_out_time from car WHERE license_plate = :license_plate;");
    q.bindValue(":license_plate", license_plate);
    q.exec();
    q.next();
    //qDebug()<<qISin.value(0)<<qISin.value(1);
    if((!q.value(0).isNull())&&q.value(1).isNull()) //如果车入库时间非空，出库未空则未出场
    {
        //qDebug()<<q.value(0);
        //qDebug()<<q.value(1);
        QMessageBox::information(this,"入场失败","车辆仍未出场");
        ui->Car_idinput->clear();
        return ;
    }
    //qDebug()<<q.value(0);
    //qDebug()<<q.value(1);

    //获取当前的时间
    // 获取当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    // 将时间转换为字符串格式（格式化为 "yyyy-MM-dd hh:mm:ss"）存储到MySQL的database
    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    //将信息上传到数据库
    //位置
    QString location = mysql_C.Parking_name;

    QString sql_submitCar = QStringLiteral("INSERT INTO CAR (license_plate, check_in_time,location) VALUES ('%1','%2','%3');").arg(license_plate,formattedDateTime,location);

    if(mysql_C.execute_bool(sql_submitCar))
    {
        qDebug()<<"车牌数据插入成功";
        ui->Car_idinput->clear();  //清空输入框
        QMessageBox::information(this,"停车入库","车牌入库成功!");


        mysql_C.parking_acc(); //让现有车库加一
        qDebug()<<mysql_C.parking_now_count;
        park_num();

    }
    else {
        qDebug()<<"车牌插入失败";
        qDebug()<<sql_submitCar;
        QMessageBox::information(this,"停车入库","车牌入库失败！！");

    }
}





void Car::on_messageButton_clicked()
{
    //判断摄像头和视频，如果视频暂停，隐藏，如果摄像头，隐藏即可
    if(camera->state() == QCamera::ActiveState||camera->state() == QCamera::LoadedState||player->state() == QMediaPlayer::PlayingState||player->state() == QMediaPlayer::PausedState)
    {
        player->stop(); //停止播放文件
        videowidget->setVisible(false); //隐藏文件播放的QVideoWidget
        viemfinder->setVisible(false); //隐藏摄像头显示区域
    }


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
    //读取数据库数据，将车位剩余情况显示到TextLabel当中

}
//简单展示现有的车位图
void Car::park_num()
{
    //qDebug()<<"num进入"<<mysql_C.reserve;
    //连接数据库，当前停车场名称的数据库，对应的现有停车场数量，和总停车场数量
    QString park_name = mysql_C.Parking_name;

    q.prepare("SELECT P_now_count,P_all_count,P_reserve_count FROM parking WHERE P_name = :park_name;");
    q.bindValue(":park_name", park_name);
    q.exec();
    q.next();
    QString now_count = q.value(0).toString();
    QString all_count = q.value(1).toString();
    QString reserve_count = q.value(2).toString();
    //将停车场数据呈现到图表中
    mysql mysql_instance;
    mysql_instance.reserve = reserve_count.toInt();
    mysql_instance.parking_now_count = q.value(0).toInt();
    //创建饼图
    // 创建一个QPieSeries对象并添加数据//为每个分块设置颜色
    series = new QPieSeries();
    series->append("预约数", q.value(2).toInt());
    series->append("入库数", q.value(0).toInt())->setColor("#FFA500");
    series->append("剩余数", mysql_instance.parking_count-q.value(2).toInt()-q.value(0).toInt());
    series->setHoleSize(0.3); //设置中间 空洞大小
    series->pieSize();
    //qDebug()<<mysql_instance.parking_now_count<<reserve_count.toInt()<<mysql_instance.parking_count-mysql_instance.parking_now_count-reserve_count.toInt();
    //qDebug()<<"数据库"<<now_count.toInt()<<all_count<<reserve_count;


    //为每个分块设置标签文字
    for(int i = 0;i<=2;i++)
    {
        slice = series->slices().at(i); //获取分块

        slice->setLabel(slice->label()+QString::asprintf(":%.0f",slice->value()));
        if(slice->value()<10)
        {
            slice->setLabelVisible(false);
        }
        else {
            slice->setLabelVisible(true);

        }


        connect(slice,SIGNAL(hovered(bool)),this,SLOT(on_PieSliceHighlight(bool)));

    }
    slice->setExploded(true); //最后一个设置为exploded,设置分裂效果
    slice->setExplodeDistanceFactor(0.1);


    // 创建一个QChart对象，并将QPieSeries对象添加到图表中
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(park_name);
    chart->legend()->setVisible(true);//图例
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setBackgroundBrush(QBrush(QColor(255, 255, 255))); //设置背景为白色


    // 创建一个QChartView对象并设置图表
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(532,371);
    chartView->backgroundBrush();

    // 将QChartView对象转换为QPixmap对象
    QPixmap pixmap = chartView->grab();

    // 将QPixmap对象显示到QLabel中
    ui->label_pie->setPixmap(pixmap);



}

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
    QSqlQuery q = mysql_C.execute(sql_fee);
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

void Car::on_PieSliceHighlight(bool show)
{
    //鼠标移入，移出时发射hovered()信号，动态设置exploed效果
    QPieSlice* slice = static_cast<QPieSlice *>(sender());
    slice->setExploded(show);
}
//当出库操作之后
void Car::on_DeleteCar_clicked()
{
    //读取车牌号
    QString license_plate = ui->Car_output->text(); //读取输入或识别出来的车牌号
    if(!checkPlateNumber(license_plate))
    {
        //QMessageBox::information(this,"识别失败","您输入的车牌号不合规");
        return;
    }
    //先检查车牌是否已入库，若未入库则报错，并返回时间
    QString sqlplate_check= QStringLiteral("select id from CAR where license_plate = '%1' AND check_out_time IS NULL; ").arg(license_plate);
    QSqlQuery query =mysql_C.execute(sqlplate_check);
    if(!query.size()) //如果不存在此数据
    {
         QMessageBox::information(this,"入库检测","当前车牌不在车库当中!!");
         return ;
    }
    //获取当前车牌号的时间
    QString sql_time= QStringLiteral("select check_in_time from CAR where license_plate = '%1' ORDER BY check_in_time DESC;").arg(license_plate);
    QSqlQuery q = mysql_C.execute(sql_time);
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

    q.prepare("UPDATE CAR SET check_out_time=:check_out_time, fee=:fee WHERE license_plate=:license_plate");
    q.bindValue(":check_out_time", formattedDateTime);
    q.bindValue(":fee", fee);
    q.bindValue(":license_plate", license_plate);

    if(q.exec())
    {
        qDebug()<<"车辆出库成功";
        ui->Car_output->clear();  //清空输入框
        QString message = QString("出库成功，车牌号'%1'需要支付'%2'元").arg(license_plate, QString::number(fee, 'f', 2));

        QMessageBox::information(this,"停车出库库",message);
        //车牌插入成功后，更新车库数据
        mysql_C.parking_dec(); //让现有车位数量减一
        park_num();

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

        if (mysql_C.execute_bool(sql_Drop_car))
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

void Car::video_Init()
{
    //本地视频播放初始化
    //初始化

    player = new QMediaPlayer(this);
    //显示的窗体
    videowidget = new QVideoWidget(this);
//    videowidget->resize(500,250);
    videowidget->setGeometry(20, 10, 500, 282); //设置窗口位置和大小
    //播放视频
    player->setVideoOutput(videowidget);   //设置输出到哪里


    //获取视频总时长
    connect(player,&QMediaPlayer::durationChanged,this,&Car::Get_Duration);

    connect(player, &QMediaPlayer::positionChanged, this, &Car::On_Position_Changed);
    //Open File为打开窗体名称

    //让滑动与视频时长产生关联
    connect(ui->horizontalSlider,&QSlider::valueChanged,this,&Car::Slider_Changed);
}

void Car::camera_Init()
{
    //摄像头初始化
    //输出所有摄像头信息
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if(cameras.count() > 0)
    {
        foreach (const QCameraInfo &cameraInfo, cameras)
            qDebug() << cameraInfo.deviceName();
        camera = new QCamera(cameras.at(0));  //编号为第几个摄像头
    }
    viemfinder = new QCameraViewfinder(this); //创建显示的区域
    camera->setViewfinder(viemfinder);  //显示出摄像头捕获的画面
    //设置摄像头显示的大小
    //viemfinder->resize(500,260);
    viemfinder->setGeometry(10, 10, 500, 250); //设置窗口位置和大小
    //用户摄像头截图
    imageCapture = new QCameraImageCapture(camera);
}

//打开文件
void Car::on_fileopen_clicked()
{


    //判断摄像头是否工作
    //判断摄像头是否初始化

        // camera已经被定义
    if(camera->state() == QCamera::ActiveState)
    {
        camera->stop(); //停止摄像头
        viemfinder->setVisible(false); //隐藏摄像头显示区域
        player->setVideoOutput(videowidget);//将显示区域设置为文件播放的QVideoWidget
        //delete_video_Init();
        videowidget->setVisible(true); //隐藏文件播放的QVideoWidget
    }
    fileName = QFileDialog::getOpenFileName(this,"选择播放的视频文件","..\\"); //打开文件(当前文件夹)放到fileName
    //将打开的文件作为视频播放的来源
    player->setMedia(QUrl::fromLocalFile(fileName)); //作为player的来源
    //获取视频时长
    player->play();  //播放

}
//视频播放
void Car::on_video_start_clicked()
{
//    qDebug()<<camera->state();
    if((camera->state() == QCamera::LoadedState) &&(cameraPaused == true) )
    {
        camera->start(); //摄像头开启
        cameraPaused = false;
    }
    else
    {
        player->play(); //视频播放
    }

}
//暂停

void Car::on_video_pause_clicked()
{
    if(camera->state() == QCamera::ActiveState)
    {
        camera->stop(); //摄像头暂停
        cameraPaused = true; //设置状态变量为真
    }
    else
    {
        player->pause(); //视频播放暂停
    }

}

void Car::Get_Duration()
{
    //获取视频时长
    //qDebug()<<player->duration(); //持续时间
    //设置进度条最大值
    ui->horizontalSlider->setMaximum(int(player->duration()));
}

void Car::Slider_Changed()
{
    pos = ui->horizontalSlider->value(); //获取当前的值
    player->setPosition(pos);  //改变播放位置
}
void Car::On_Position_Changed(qint64 position)
{
    ui->horizontalSlider->setValue(int(position)); // 更新进度条的值
}

//画面捕获，拍照
//使用easyPR将车牌号放到固定区域，然后将识别的照片放到入库和出库的位置
void Car::on_camera_take_clicked()
{
    //如果摄像头播放
    if(camera->state() == QCamera::ActiveState)
    {
        camera->setCaptureMode(QCamera::CaptureStillImage);  //捕获图片
        //弹出标准对话框
        //QString fileName = QFileDialog::getSaveFileName(nullptr, QString(), QString(), QString(), nullptr, QFileDialog::DontConfirmOverwrite); //保存的文件名字

        imageCapture = new QCameraImageCapture(camera);  //创建一个用于捕获图片的对象

      //当摄像头成功捕获一张图片后进行车牌识别
      connect(imageCapture, &QCameraImageCapture::imageCaptured, this, [=](int /*id*/, const QImage& img)
      {
          Mat src;
          Mat rgbImg;
          //src = Mat(img.height(), img.width(), CV_8UC4, (uchar*)img.bits(), img.bytesPerLine()).clone();
          src = Mat(img.height(), img.width(), CV_8UC4, const_cast<unsigned char*>(img.bits()), static_cast<size_t>(img.bytesPerLine())).clone();

          cvtColor(src, rgbImg, CV_BGRA2RGB);  //将OpenCV中颜色通道排列方式与Qt中不同的图片格式转换为Qt中正常的图片格式
          //QImage qImg(rgbImg.data, rgbImg.cols, rgbImg.rows, rgbImg.step, QImage::Format_RGB888);
          QImage qImg(rgbImg.data, rgbImg.cols, rgbImg.rows, static_cast<int>(rgbImg.step), QImage::Format_RGB888);

          //缩放图片到指定大小
          QSize newSize(200,100);
          QImage scaledImg = qImg.scaled(newSize,Qt::KeepAspectRatio);

          //使用easyPR获取车牌信息
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
              ui->screen_label->setPixmap(QPixmap::fromImage(qPlateImg));

              QString plateStr = QString::fromLocal8Bit(plate.getPlateStr().c_str());
              // 获取车牌号码的部分字符串
              QStringList list = plateStr.split(":");
              if (list.size() >= 2) {
                  plateStr = list[1].trimmed();
              }
              ui->Car_idinput->setText(plateStr);
              ui->Car_output->setText(plateStr);
              qDebug()<<plateStr;
              std::cout << "plate: " << plate.getPlateStr() << std::endl;
          }


      });

        //开始进行捕获
        imageCapture->capture(fileName);  //捕获图片，保存到要保存的目录为上面对话框设定的目录
    }
    //如果视频播放
    else
    {
        //使用OpenCV截取图片
        //获取当前帧的时间
        qint64 currentTime = player->position();
        //将播放器跳转到当前时间
        player->setPosition(currentTime);
        //获取当前媒体文件的URL地址
        //QString filePath = player->currentMedia().canonicalUrl().toString();
        //获取当前媒体文件的本地文件路径
        QString filePath = player->currentMedia().canonicalUrl().toLocalFile();
        //使用OpenCV库读取该路径下的视频
        VideoCapture cap(filePath.toStdString());
        //
        if(!cap.isOpened())
        {
            qDebug()<<"Failed to open file:"<<filePath;
            return ;
        }
        //跳转到当前时间
        cap.set(CAP_PROP_POS_MSEC,currentTime);
        Mat  frame;
        cap.read(frame);
        //将获取到的图片显示在QLabel上供用户选择保存位置
        QImage img(static_cast<const uchar*>(frame.data),frame.cols,frame.rows,QImage::Format_RGB888);
        //缩放图片到指定大小
        QSize newSize(200,100);
        QImage scaledImg = img.scaled(newSize,Qt::KeepAspectRatio);


        //使用easyPR获取车牌信息
        std::vector<easypr::CPlate> plates;
        m_plateRecognize.plateRecognize(frame,plates,0);
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
            ui->screen_label->setPixmap(QPixmap::fromImage(qPlateImg));

            QString plateStr = QString::fromLocal8Bit(plate.getPlateStr().c_str());
            // 获取车牌号码的部分字符串
            QStringList list = plateStr.split(":");
            if (list.size() >= 2) {
                plateStr = list[1].trimmed();
            }
            ui->Car_idinput->setText(plateStr);
            ui->Car_output->setText(plateStr);
            qDebug()<<plateStr;
            std::cout << "plate: " << plate.getPlateStr() << std::endl;
        }

        //ui->screen_label->setPixmap(QPixmap::fromImage(scaledImg));







        // 保存整个屏幕为QPixmap
//            QScreen *screen = QGuiApplication::primaryScreen();
//            //QString filePathName = QFileDialog::getSaveFileName(); //保存的文件名字
//            QString filePathName = "cut-";
//            filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
//            filePathName += ".png";
//            pixmap = screen->grabWindow(0);
//            if(!pixmap.save(filePathName,"png"))
//            {
//                qDebug()<<"cut save png failed"<<endl;
//            }
//            else {
//                qDebug()<<"cut save png successful"<<endl;
//                // 计算视频的位置和大小
//                qDebug()<<"Full pixmap width: "<<pixmap.width()<<" height: "<<pixmap.height()<<endl;
//                QRect geo = this->geometry();
//                QRect appGeo = geo; // 整个应用程序在图片中的位置。
//                qDebug()<<"App x: "<<geo.x()<<" y: "<<geo.y()<<" width: "<<geo.width()<<" height: "<<geo.height()<<endl;

//                geo = videowidget->geometry(); // 播放视频在图片中的位置。
//                qDebug()<<"VW x: "<<geo.x()<<" y: "<<geo.y()<<" width: "<<geo.width()<<" height: "<<geo.height()<<endl;

//                //QWidget *centerWidget = centralWidget(); // QMainWindow在应用程序的位置

//                // 假设非主窗口是由主窗口创建的
//                // 在非主窗口中获取指向主窗口实例的指针
//                QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent());
//                if (mainWindow)
//                {
//                    // 获取指向中心窗口部件的指针
//                    QWidget *centerWidget = mainWindow->centralWidget();
//                    // 执行相关操作
//                    QRect centerRect = centerWidget->geometry();
//                    qDebug()<<"center x: "<<centerRect.x()<<" y: "<<centerRect.y()<<" width: "<<centerRect.width()<<" height: "<<centerRect.height()<<endl;

//                    QRect copyGeo;
//                    copyGeo.setX(geo.x() + appGeo.x() + centerRect.x()); // x=三个x相加
//                    copyGeo.setY(geo.y() + appGeo.y() + centerRect.y());
//                    copyGeo.setWidth(geo.width());
//                    copyGeo.setHeight(geo.height());
//                    qDebug()<<"VW1 x: "<<copyGeo.x()<<" y: "<<copyGeo.y()<<" width: "<<copyGeo.width()<<" height: "<<copyGeo.height()<<endl;

//                    QPixmap pixmapCopy = pixmap.copy(copyGeo); // copy图片
//                    filePathName.prepend("Copy+");

//                    //QString fileName = QFileDialog::getSaveFileName(); //保存的文件名字
//                    if(!pixmapCopy.save(fileName,"png"))
//                    {
//                        qDebug()<<"copy cut save png failed"<<endl;
//                    }
//                    else {
//                        qDebug()<<"copy cut save png successfull "<<endl;
//                    }
//                }
//                else
//                {
//                    qDebug()<<"mainwindow error";
//                }
//            }

    }

}


//点击摄像头后，摄像头开始工作
void Car::on_camera_button_clicked()
{

    if(player->state() == QMediaPlayer::PlayingState||player->state() == QMediaPlayer::StoppedState||player->state() == QMediaPlayer::PausedState)
    {
        player->stop(); //停止播放文件
        videowidget->setVisible(false); //隐藏文件播放的QVideoWidget
        camera->setViewfinder(viemfinder); //将显示区域设置为摄像头捕获的QCameraViewfinder
    }
    viemfinder->setVisible(true); //显示摄像头显示区域
    camera->start();  //让摄像头开始工作
}

//当 点击主页判断状态
void Car::on_MainButton_clicked()
{
    //判断摄像头和视频，如果视频暂停，隐藏，如果摄像头，隐藏即可
    if(camera->state() == QCamera::ActiveState||camera->state() == QCamera::LoadedState)
    {

        viemfinder->setVisible(true); //隐藏摄像头显示区域
    }
    else
    {
        //player->stop(); //停止播放文件
        videowidget->setVisible(true); //隐藏文件播放的QVideoWidget
    }
    park_num();
}
//，若更新，则更新饼图，直接更新
void Car::checkMySQLData()
{
    QString park_name = mysql_C.Parking_name;

    q.prepare("SELECT P_reserve_count FROM parking WHERE P_name = :park_name;");
    q.bindValue(":park_name", park_name);
    q.exec();
    q.next();
    int reserve_count = q.value(0).toInt();
    if(reserve_count!=mysql_C.reserve)//如果数据库预约数与本地不同，则进行同步
    {
         park_num();
    }
    else
    {
        return ;
    }

}
