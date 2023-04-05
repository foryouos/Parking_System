#include "mysql.h"

mysql::mysql()
{
    //如果数据库没有连接则连接数据库,解决 重复连接问题 ，QT会 默认连接，只有关闭所有窗口才会关闭连接
    //唯一的数据库连接qt_sql_default_connection会以此命名，若包含则不连接
    if(!QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::addDatabase("QMYSQL");

                db.setHostName("localhost");  //连接本地主机
                db.setPort(3306);
                db.setDatabaseName("Car");
                db.setUserName("root");
                db.setPassword("5211314");
                bool ok = db.open();
                if (!ok){
                    qDebug()<<"mysql open error";
                    //QMessageBox::information(this, "infor", "link success");
                }
    }

}

mysql::~mysql()
{

}

void mysql::create_user()
{
    QString createsql = QString("CREATE TABLE IF NOT EXISTS USER ( "
                                 "id INT PRIMARY KEY AUTO_INCREMENT, "
                                 "username VARCHAR(255) UNIQUE NOT NULL, "
                                 "password VARCHAR(255) NOT NULL, "
                                 "telephone VARCHAR(11) NOT NULL, "
                                 "truename VARCHAR(255) NOT NULL"
                                 ");");

    if(!execute(createsql).exec()) //执行创建表格语句
    {
        qDebug()<<"table create error";
    }
    else
    {
        qDebug()<<"table create success";
    }
}

QSqlQuery mysql::execute(QString createsql)
{
    //创建执行语句对象
    QSqlQuery query;
    if(!query.exec(createsql))
    {
        qDebug()<<"MySQl Execute error";
    }
    else
    {
        qDebug()<<"MySQl Execute success";
    }
    return query;
}

bool mysql::execute_bool(QString createsql)
{
    //创建执行语句对象
    QSqlQuery query;
    if(!query.exec(createsql))
    {
        qDebug()<<"MySQl Execute error";
        return false;
    }
    else
    {
        qDebug()<<"MySQl Execute success";
        return true;
    }
}



void mysql::mysql_close()
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        //关闭数据库连接
        db.close();
    }

}
void mysql::beginTransaction() {
    db.transaction();
}

// 提交事务
void mysql::commitTransaction() {
    db.commit();
}

// 回滚事务
void mysql::rollbackTransaction() {
    db.rollback();
}
