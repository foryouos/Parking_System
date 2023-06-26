#ifndef MYSQLINIT_H
#define MYSQLINIT_H
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
#include "mysql.h"
//执行MySQL初始化语句
/* 初始化表格，
 * 初始化数据库表关系
 * 初始化数据库数据
 * 初始化触发器
*/
class MySQLInit : public QObject
{
    Q_OBJECT
public:
    explicit MySQLInit(QObject *parent = nullptr);
    ~ MySQLInit();
    void create_user(); //创建管理员用户表格
    void create_car();  //存放车辆信息
    void create_parking(); //创建停车场表
    void Parking_init(QString Parking_name);  //初始化停车库数据
    void parking_reserve(); //建立车位预定信息表，在数据库初始化完毕时创建
signals:

public slots:

private:
    mysql *mysqlc;
};

#endif // MYSQLINIT_H
