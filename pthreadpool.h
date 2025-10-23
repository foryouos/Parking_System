#pragma once
// 数据库连接池 供数据库连接池

#include <QRunnable>
#include <QObject>
#include "mysql.h"
//创建子线程 // 车牌号线程池
class PthreadPool_Plate : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit  PthreadPool_Plate(QObject *parent = nullptr);
    void RecvPlate(mysql* mysql,QString license_plate, QString formattedDateTime, QString location);
    // 在子类中必须要重写的函数，里面的任务是处理流程
    void run() override;
public slots:
    //接受数据
    void RecvMess();

signals:
    void RecvMessFinished();
    //使用信号槽将数据传递给主线程，操作函数由主线程进行
    void Plate_up_signal(bool up);

public slots:

private:

};


