#include "mysql.h"
#include "initfile.h"
mysql::mysql(QObject *parent) : QObject(parent)
{
    mysql_init();
}

mysql::mysql(const mysql &other)
{
    qDebug()<<"进入复制构造函数";
    mysql_init();
}

mysql::~mysql()
{

}

void mysql::mysql_init()
{
    //使用初始化后的数据
    QString filePath =QApplication::applicationDirPath() + "/parkinginit.json";
    QFile file(filePath);
    //如果可以打开文件并读取文件
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray jsonData = file.readAll();
        QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
        QJsonObject jsonObj = jsonDoc.object();
        //qDebug()<<"文件存在并打开文件!";
        QString ip = jsonObj["IP"].toString();
        int port = jsonObj["port"].toInt();
        QString user = jsonObj["account"].toString();
        QString password = jsonObj["password"].toString();
        QString DatabaseName = jsonObj["database"].toString();

        Parking_name=jsonObj["park_name"].toString();
        p_fee = jsonObj["P_fee"].toInt();
        parking_count =jsonObj["P_count"].toInt(); //总量
        reserve = 0;


        //中容量

        //如果数据库没有连接则连接数据库,解决 重复连接问题 ，QT会 默认连接，只有关闭所有窗口才会关闭连接
        //唯一的数据库连接qt_sql_default_connection会以此命名，若包含则不连接
        if(!QSqlDatabase::contains("qt_sql_default_connection"))
        {
            db = QSqlDatabase::addDatabase("QMYSQL");

                    db.setHostName(ip);  //连接本地主机
                    db.setPort(port);
                    db.setDatabaseName(DatabaseName);
                    db.setUserName(user);
                    db.setPassword(password);
                    bool ok = db.open();
                    if (!ok)
                    {
                        qDebug()<<"mysql open error";
                        //QMessageBox::information(this, "infor", "link success");
                    }
                    else
                    {
                        qDebug()<<"mysql connect successful";
                    }
        }
    }
}
bool mysql::Parking_is_NULL(QString Parking_name)
{
    QString createsql_check= QStringLiteral("select P_id from parking where P_name = '%1'; ").arg(Parking_name);
    QSqlQuery query =mysql().execute(createsql_check);
    if(query.size() == 0)//如果不存在此数据
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool mysql::Is_connection()
{
    if(db.open())
    {
        return true;
    }
    else
    {
        return false;
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
void mysql::beginTransaction()
{
    db.transaction();
}

// 提交事务
void mysql::commitTransaction()
{
    db.commit();
}

// 回滚事务
void mysql::rollbackTransaction()
{
    db.rollback();
}
//实现停车位现有车位减+1
void mysql::parking_acc()
{
    QString location = mysql().Parking_name;
    QString createsql_Parking = QStringLiteral("UPDATE parking SET P_now_count = P_now_count + 1 WHERE P_name = '%1';").arg(location);

    if(mysql().execute_bool(createsql_Parking))
    {
        parking_now_count++;
        qDebug()<<"Parking Acc successful"<<parking_now_count;
    }
    else {
        qDebug()<<"Parking ACC Error";
    }
}
//实现停车场现有车位-1
void mysql::parking_dec()
{
    QString location = mysql().Parking_name;
    QString createsql_Parking = QStringLiteral("UPDATE parking SET P_now_count = P_now_count - 1 WHERE P_name = '%1';").arg(location);

    if(mysql().execute_bool(createsql_Parking))
    {
        parking_now_count--;
        qDebug()<<"Parking dec successful";
    }
    else {
        qDebug()<<"Parking dec Error";
    }
}


QSqlDatabase mysql::get_db()
{
    return db;
}

