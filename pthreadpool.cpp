#include "pthreadpool.h"
#include <QElapsedTimer> //记录时间
#include <QtDebug>
#include <QThread>
// 车牌号线程池
PthreadPool_Plate::PthreadPool_Plate(QObject *parent) : QObject(parent),QRunnable ()
{
    // 任务执行完毕，该对象自动销毁
    setAutoDelete(false);
}

void PthreadPool_Plate::RecvPlate(mysql* mysql,QString license_plate, QString formattedDateTime, QString location)
{

}

void PthreadPool_Plate::run()
{

}
void PthreadPool_Plate::RecvMess()
{

}
