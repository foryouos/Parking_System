#include "initfile.h"
#include "ui_initfile.h"

initFile::initFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::initFile)
{
    ui->setupUi(this);
    setWindowTitle("停车场初始化");
}

initFile::~initFile()
{
    delete ui;
}
//当点击提交时:将用户所输入的数据提交到文件当中
void initFile::on_submit_init_clicked()
{
     //设置json对象，将所有的数据存储到本地
    QString filePath =QApplication::applicationDirPath() + "/parkinginit.json";
    QFile file(filePath);
    //如果可以打开文件并读取文件
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug()<<"文件存在并打开文件!";
        return ;

    }
    else //如果不能打开文件，将写入的数据序列化为JSON格式的文本,保存到本地文件中
    {
        //读取所有输入的数据
        QString IP = ui->IP->text();  //读取IP地址
        int PORT = ui->PORT->text().toInt(); //端口号
        QString database = ui->database->text(); //数据库名
        QString account = ui->account->text(); //账户名
        QString password = ui->password->text(); //密码
        QString park_name = ui->parking_name->text(); //停车场名称
        int P_fee = ui->park_fee->text().toInt();  //费用
        int P_count = ui->park_count->text().toInt();//停车场总容量
        qDebug()<<IP<<PORT<<database<<account<<password<<park_name<<P_fee;
        //以写的方式打开文件
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            //创建JSON类
            QJsonObject jsonObj;
            //插入数据
            jsonObj.insert("IP",IP);
            jsonObj.insert("port",PORT);
            jsonObj.insert("database",database);
            jsonObj.insert("account",account);
            jsonObj.insert("password",password);
            jsonObj.insert("park_name",park_name);
            jsonObj.insert("P_fee",P_fee);
            jsonObj.insert("P_count",P_count);
            //写入
            QJsonDocument jsonDoc(jsonObj);
            QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);
            file.write(jsonString.toUtf8());
            file.close();
            //首次启动的表格，触发器初始化
            mysql(); //mysql初始化
            mysql().create_car();  //初始化车库表格
            //建立车库预定表
            mysql().parking_reserve(); //建立车库预定表，并生成触发器
            //初始化停车表
            mysql().create_parking(); //创建车表

            mysql().Parking_init(); //初始化车表数据

            mysql().create_user(); //初始化用户表格

            //文件存在启动主程序
            MainWindow *w = new MainWindow;
            w->show();
            //关闭当前窗口
            this->close(); //关闭当前窗口
        }
    }

}
