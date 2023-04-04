#include "signup.h"
#include "ui_signup.h"
#include "mainwindow.h" //引入登陆窗口
Signup::Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
    setWindowTitle("停车场管理系统注册");
    //设置左侧背景图片
    QPixmap *pix = new QPixmap(":./images/welcome.png");
    QSize sz = ui->label_login_image->size();
    ui->label_login_image->setPixmap(pix->scaled(sz));
}

Signup::~Signup()
{
    delete ui;
}

void Signup::on_pushButton_back_clicked()
{
    //创建登陆主页面
    MainWindow * w =new MainWindow;
    //呈现登陆主页面
    w->show();
    //关闭当前的注册页面
    this->close();
}

//点击注册页面的确认按钮
void Signup::on_pushButton_sure_clicked()
{
    //初始化数据库连接
    //sqlite_Init();
    mysql_Init();
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString surepass = ui->lineEdit_sure_password->text();
    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "注册认证", "账户和密码不能为空");
        return;
    }
    //判断确认密码是够一致
    if(password == surepass)
    {
        QString encryptedPassword = encryptPassword(password); // 对密码进行加密
        //插入语句
        QString sql = QStringLiteral("insert into USER(username,password) values('%1','%2');").arg(username, encryptedPassword);
        // QString sql=QString("insert into user(username,password) values('%1','%2');").arg(username).arg(password);
        //创建执行语句
        QSqlQuery query;
        //判断执行结果
        if(!query.exec(sql))
        {
            qDebug()<<"insert into error";
        }
        else
        {
            qDebug()<<"inert into success";
            QMessageBox::information(this,"注册成功","注册账号成功!");
            //创建主登陆页面的堆空间，进入登陆页面，释放注册页面空间
            MainWindow *w = new MainWindow;
            w->show();
            this->close();
        }
    }
    else
    {
        QMessageBox::information(this,"注册认证","两次密码输入不一致");
    }
}

