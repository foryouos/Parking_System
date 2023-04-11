#ifndef MYSQL_H
#define MYSQL_H
#include <QSqlQuery> //数据库执行语句
#include <QMessageBox> //数据库执行语句
#include <QDebug>
#include <QJsonDocument>  //保存JSON数据
#include<QSqlError> //mysql报错语句
#include <iostream>
#include <QFile> //读取本地json数据
#include <QApplication>
#include <QJsonObject>
#include <QResource>
class mysql
{
private:
    QSqlDatabase db; //链接的数据库
public:
    mysql(); //初始化连接数据库
    ~mysql();// 析构语句

    QString Parking_name; //位置
    int parking_count;  //总停车位
    int parking_now_count; //现有停车位
    int park_reserve;  //预定的车位数
    int p_fee; //车位单价

    void create_user(); //创建管理员用户表格
    void create_car();  //存放车辆信息

    void create_parking(); //创建停车场表

    void Parking_init();  //初始化停车库数据

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

    //停车场车位减一，
    void parking_acc();
    //停车场车位加一
    void parking_dec();

    //建立车位预定信息表，在数据库初始化完毕时创建
    void parking_reserve();

};

#endif // MYSQL_H
