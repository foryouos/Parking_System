#include "updatapie.h"

UpdataPie::UpdataPie(QObject *parent) : QObject(parent)
{

}
void UpdataPie::park_num()
{

    QString park_name = Parking_name;
    q.prepare("SELECT P_now_count,P_all_count,P_reserve_count FROM parking WHERE P_name = :park_name;");
    q.bindValue(":park_name", park_name);
    q.exec();
    q.next();
    QString now_count = q.value(0).toString();
    QString all_count = q.value(1).toString();
    QString reserve_count = q.value(2).toString();
    //将停车场数据呈现到图表中
    reserve = reserve_count.toInt();
    //调用信号更新数据 使其主线程调用create_pie函数
    emit PieChanged(reserve_count.toInt());  //仅改变预约量
}
void UpdataPie::checkMySQLData()
{
    QString park_name = Parking_name;

    q.prepare("SELECT P_reserve_count FROM parking WHERE P_name = :park_name;");
    q.bindValue(":park_name", park_name);
    q.exec();
    q.next();
    int reserve_count = q.value(0).toInt();
    if(reserve_count!=reserve)//如果数据库预约数与本地不同，则进行同步
    {
        //更新mysql的全局数据
         park_num();
    }
    else
    {
        return ;
    }
}
//传入三个参数 名字，reserve,now_count_L
void UpdataPie::CheckPie(QString m_Parking_name,int m_reserve,int m_now_count_L)
{
    Parking_name = m_Parking_name;
    reserve = m_reserve;
    now_count_L = m_now_count_L;
    while(update)
    {
        checkMySQLData();
        QThread::msleep(1000); // 暂停 1 秒钟
    }
}

