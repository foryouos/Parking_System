#ifndef MYSQL_H
#define MYSQL_H
#include <QSqlQuery> //数据库执行语句
#include <QMessageBox> //数据库执行语句
#include <QDebug>
#include <QJsonDocument>  //保存JSON数据
#include <QSqlError> //mysql报错语句
#include <iostream>
#include <QFile> //读取本地json数据
#include <QApplication>
#include <QJsonObject>
#include <QResource>
#include <QObject>

class mysql : public QObject
{
    Q_OBJECT
public:
    explicit mysql(QObject *parent = nullptr); //初始化连接数据库
    // QObject 的复制构造函数是是有的，默认无法在子类中使用
    mysql(const mysql& other); // 声明复制构造函数
    mysql& operator=(const mysql& other) = delete; // 禁用赋值运算符


    ~mysql();// 析构语句
    // mysql封装语句
    // 判断当前停车场是否为空
    void mysql_init();
    bool Parking_is_NULL(QString Parking_name);
    bool Is_connection();
    QString Parking_name; //位置
    int parking_count;  //总停车位
    int parking_now_count; //现有停车位
    int reserve;  //预定的车位数
    int p_fee; //车位单价
    //可以执行QT中MySQL的增删改查和建表操作
    QSqlQuery execute(QString createsql);//执行mysql语句,参数为执行语句

    bool execute_bool(QString createsql);
    void mysql_close(); //关闭mysql数据库连接

    // 开始事务
    void beginTransaction();
    // 提交事务
    void commitTransaction();
    // 回滚事务
    void rollbackTransaction();


    //获取数据库连接
    QSqlDatabase get_db();
signals:
    void now_count_changed_signal(int parking_now_count);

public slots:
    //停车场车位减一，
    void parking_acc();
    //停车场车位加一
    void parking_dec();
private:
    QSqlDatabase db; //链接的数据库
};

#endif // MYSQL_H
