#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QCryptographicHash> //加载加密函数库
#include "signup.h"
#include "car.h"
//加密函数，输入密码，返回hash值
QString encryptPassword(QString password) {
    QByteArray passwordBytes = password.toUtf8(); // 将密码转换为字节数组
    QByteArray hashBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256); // 使用SHA256算法进行加密
    QString hash = QString::fromLatin1(hashBytes.toHex()); // 将加密后的字节数组转换为十六进制字符串
    return hash;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口标题
    setWindowTitle("停车场管理系统登录");
    //初始化sqlite数据库
    //sqlite_Init();
    mysql(); //mysql初始化
    mysql().create_user(); //初始化用户表格


    //设置登陆页面左边背景图
    // 创建QPixmap对象，加载png图像存储到pix变量中，使用new关键字分配堆内存来储存该对象
    QPixmap *pix = new QPixmap(":/images/hello.jpg");
    QSize sz = ui->label_image->size();
    ui->label_image->setPixmap(pix->scaled(sz));

    //设置图片阴影效果
    //QGraphicsDropShadowEffect对象添加阴影效果，
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影效果的偏移量，并不在垂直方向发生任何偏移
    shadow->setOffset(-3,0);
    //设置阴影的颜色
    QColor color(227, 180, 184);
    shadow->setColor(color);
    // 设置模糊半径，数值越大，阴影边缘越模糊
    shadow->setBlurRadius(30);
    ui->label_image->setGraphicsEffect(shadow);
    connect(ui->lineEdit_password, &QLineEdit::returnPressed, ui->pushButton_2, &QPushButton::click);

}

MainWindow::~MainWindow()
{
    mysql().mysql_close(); //关闭窗口则私房数据库连接
    delete ui;

}

//登录
void MainWindow::on_pushButton_2_clicked()
{

    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("账户和密码不能为空");
        msgBox.setWindowTitle("登陆");
        msgBox.setStyleSheet("background-color: white; color: red;");
        msgBox.exec();
        return;
    }
    QString encryptedPassword = encryptPassword(password); // 对密码进行加密
    QString sql = QStringLiteral("select * from USER WHERE username='%1' and password = '%2'").arg(username,encryptedPassword);

    //判断执行结果
    if(!mysql().execute(sql).next())
    {
        qDebug()<<"Login error";
        QMessageBox::information(this,"登陆认证","账户或密码错误");
    }
    else
    {
        qDebug()<<"Login success";

        //QMessageBox::information(this,"登陆认证","登陆成功");
        //登陆成功后跳转到其它页面
        //QWidget *W = new QWidget;
        //W->show();


        //创建登陆主页面
        Car * w =new Car;
        //呈现Car主页面
        w->show();
        //关闭当前的注册页面
        this->close();

    }

}

//点击返回后
void MainWindow::on_return_2_clicked()
{

    this->close();
    Signup *s = new Signup;
    s->show();
}

