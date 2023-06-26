#ifndef UPDATAPIE_H
#define UPDATAPIE_H

#include <QObject>
#include "mysql.h" //调用mysql存储的数据
#include <QThread>
// 更新饼图会在子线程一直在while循环每隔一秒运行一此，当数据发生发生变化，调用更新图片
class UpdataPie : public QObject
{
    Q_OBJECT
public:
    explicit UpdataPie(QObject *parent = nullptr);
    void CheckPie(QString m_Parking_name,int m_reserve,int m_now_count_L);
signals:
    //发出数据改变信号
    void PieChanged(int reserve);
public slots:

private:
    void checkMySQLData(); //检查数据
    void park_num(); //更新停车全局数据

    mysql* m_mysql;
    QSqlQuery q;
    bool update = true;  //若用户不在首页，不更新

    QString Parking_name;
    int reserve;
    int now_count_L;

};

#endif // UPDATAPIE_H
