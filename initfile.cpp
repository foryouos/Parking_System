#include "initfile.h"
#include "ui_initfile.h"
#include "pthreadpool.h"
#include <QTextEdit>
initFile::initFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::initFile)
{
    ui->setupUi(this);
    setWindowTitle("停车场初始化");
    //设置label垂直布局
    QString str = ui->label->text();
    ui->label->setText(str.split("",QString::SkipEmptyParts).join("\n"));
    ui->label->setAlignment(Qt::AlignCenter);
}

initFile::~initFile()
{
    delete list; //清除申请的空间
    delete mysql_c;
    delete Init;
    delete ui;
}
bool initFile::File_MySQLInit()
{

    mysql_c = new mysql;  //初始化数据库连接
    //mysql(); //连接mysql
    if(mysql_c->Is_connection())
    {
        qDebug()<<"数据库连接成功";

    }
    else {
        qDebug()<<"数据库未连接";
    }
    Init = new MySQLInit;  //初始化数据库连接
    //首次启动的表格，触发器初始化
    QTextEdit *editList = new QTextEdit(this) ;
    //设置样式
    editList->setAlignment(Qt::AlignCenter);
    editList->setTextColor(Qt::yellow);
    editList->setTextBackgroundColor(Qt::transparent); //背景透明
    editList->show();
    editList->append("初始化ing");
    Init->create_car();  //初始化车库表格

    editList->append("初始化表格完成");
    Init->create_parking(); //创建车表
    editList->append("初始化车表表完成");
    //建立车库预定表
    Init->parking_reserve(); //建立车库预定表，并生成触发器
    editList->append("初始化车辆预定表完成");
    //初始化停车表
    Init->Parking_init(list->park_name); //初始化车表数据
    editList->append("初始化车表数据完成");
    Init->create_user(); //初始化用户表格
    editList->append("初始化用户登录表完成");
    //验证执行
    editList->close();

    delete editList;
    return mysql_c->execute_bool("select * from user;");
}
void initFile::Json_Save(initFile::InitMySqlFileList *list)
{
    //设置json对象，将所有的数据存储到本地
   QString filePath =QApplication::applicationDirPath() + "/parkinginit.json";
   QFile file(filePath);
    //以写的方式打开文件
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //创建JSON类
        QJsonObject jsonObj;
        //插入数据
        jsonObj.insert("IP",list->IP);
        jsonObj.insert("port",list->PORT);
        jsonObj.insert("database",list->database);
        jsonObj.insert("account",list->account);
        jsonObj.insert("password",list->password);
        jsonObj.insert("park_name",list->park_name);
        jsonObj.insert("P_fee",list->P_fee);
        jsonObj.insert("P_count",list->P_count);
        //写入
        QJsonDocument jsonDoc(jsonObj);
        QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);
        file.write(jsonString.toUtf8());
        qDebug()<<"数据库初始化文件保存本地成功";
        file.close();
        //初始化文件

        //文件测试
        if(File_MySQLInit())
        {

            MainWindow *w = new MainWindow;
            w->show();
            //关闭当前窗口
            this->close(); //关闭当前窗口
        }
        else
        {
            qDebug()<<"数据库未连接";
            QMessageBox::information(this,"登录失败", "请检查你的初始化信息请重新输入!");
            //删除文件
            QFile::remove(filePath);
        }


    }
}

//当点击提交时:将用户所输入的数据提交到文件当中
void initFile::on_submit_init_clicked()
{
    list = new InitMySqlFileList;
    //读取所有输入的数据
    list->IP = ui->IP->text();  //读取IP地址
    list->PORT = ui->PORT->text().toInt(); //端口号
    list->database = ui->database->text(); //数据库名
    list->account = ui->account->text(); //账户名
    list->password = ui->password->text(); //密码
    list->park_name = ui->parking_name->text(); //停车场名称
    list->P_fee = ui->park_fee->text().toInt();  //费用
    list->P_count = ui->park_count->text().toInt();//停车场总容量
    qDebug()<<list->IP<<list->PORT<<list->database<<list->account<<list->password<<list->park_name<<list->P_fee;
    //将list的数据保存到Json当中
    Json_Save(list);

}
//点击关闭窗口
void initFile::on_cancel_submit_clicked()
{
    //调用确认窗口
    QMessageBox::Button btn= QMessageBox::question(this,"关闭窗口","您确定要退出关闭窗口吗？");
    //如果选择的确认则关闭
    if(btn==QMessageBox::Yes)
    {
        //关闭窗口
        this->close();
    }
    else {

    }
}

