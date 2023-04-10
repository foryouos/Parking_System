#include "mysql.h"

mysql::mysql()
{
    //读取本地json数据
    //读取文件
    QFile file(":/MySQL.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "文件打开失败:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");  // 将编码格式设置为 UTF-8
    QString jsonString = in.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject obj = doc.object();
    // 使用键来访问每一个值
    QString ip = obj["ip"].toString();
    int port = obj["port"].toInt();
    QString user = obj["user"].toString();
    QString password = obj["password"].toString();
    QString DatabaseName = obj["DatabaseName"].toString();
    //qDebug()<<ip<<port<<user<<password<<DatabaseName;


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
//创建车表
void mysql::create_car()
{
    //创建车库的表格,ID,车牌号，入库时间，出库时间，费用，id主键，车牌号索引
    QString createsql = QString("CREATE TABLE IF NOT EXISTS CAR("
                                "id INT NOT NULL AUTO_INCREMENT,"
                                "license_plate VARCHAR(20) NOT NULL,"
                                "check_in_time DATETIME NOT NULL,"
                                "check_out_time DATETIME DEFAULT NULL,"
                                "fee DECIMAL(10, 2) DEFAULT NULL,"
                                "location VARCHAR(20) NOT NULL,"
                                "PRIMARY KEY (id),"
                                "INDEX(license_plate)"
                                ");"
                            );

    if(!execute(createsql).exec()) //执行创建表格语句
    {
        qDebug()<<"table CAR create error";
    }
    else
    {
        qDebug()<<"table CAR create success";
    }
}

void mysql::create_parking()
{
    //-- 建车库表格
    // 车库id,车库名称，车库现有用量，车库总用量
    QString createsql = QString("CREATE TABLE IF NOT EXISTS PARKING("
                                    "P_id INT NOT NULL AUTO_INCREMENT,"
                                    "P_name VARCHAR(255) NOT NULL,"
                                    "P_now_count INT,"
                                    "P_all_count INT,"
                                    "P_fee DECIMAL(10, 2),"
                                    "PRIMARY KEY (P_id),"
                                    "UNIQUE INDEX(P_name)"
                                ");"
                            );

    if(!execute_bool(createsql)) //执行创建表格语句
    {
        qDebug()<<"table Parking create error";
    }
    else
    {
        qDebug()<<"table Parking create success";
    }
}
//初始化停车场数据
void mysql::Parking_init()
{
    QString createsql_check= QStringLiteral("select P_id from parking where P_name = '新科停车场'; ");
    QSqlQuery query =mysql().execute(createsql_check);
    if(!query.size()) //如果不存在此数据
    {
        qDebug()<<"检索数据为空";
        QString name = "新科停车场";
        QString all_count = "200";
        QString now_count = "0";
        QString p_fee = "30.00";
        //将停车场数据插入进入
        //QString createsql_Parking = QStringLiteral("insert into PARKING(P_name,P_now_count,P_all_count,P_fee) values('%1','%2','%3','%4');").arg(Parking_name,now_count,parking_count,p_fee);

        QString createsql_Parking = QStringLiteral("insert into PARKING(P_name,P_now_count,P_all_count,P_fee) values('%1','%2','%3','%4');").arg(name,now_count,all_count,p_fee);

        if(mysql().execute_bool(createsql_Parking))
        {
            qDebug()<<"Parking Initative successful";
        }
        else {
            qDebug()<<"Parking Initative Error";
        }
    }else
    {
        qDebug()<<"检索数据不为空";
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
//实现停车位现有车位减一
void mysql::parking_acc()
{
    QString location = "新科停车场";
    QString createsql_Parking = QStringLiteral("UPDATE parking SET P_now_count = P_now_count + 1 WHERE P_name = '%1';").arg(location);

    if(mysql().execute_bool(createsql_Parking))
    {
        qDebug()<<"Parking Acc successful";
    }
    else {
        qDebug()<<"Parking ACC Error";
    }
}
//实现停车场现有车位+1
void mysql::parking_dec()
{
    QString location = "新科停车场";
    QString createsql_Parking = QStringLiteral("UPDATE parking SET P_now_count = P_now_count - 1 WHERE P_name = '%1';").arg(location);

    if(mysql().execute_bool(createsql_Parking))
    {
        qDebug()<<"Parking dec successful";
    }
    else {
        qDebug()<<"Parking dec Error";
    }
}

