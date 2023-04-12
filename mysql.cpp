#include "mysql.h"
#include "initfile.h"
mysql::mysql()
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
        parking_count =jsonObj["P_count"].toInt();
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
                    if (!ok){
                        qDebug()<<"mysql open error";
                        //QMessageBox::information(this, "infor", "link success");
                    }
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
                                    "P_reserve_count INT,"
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
    QString park_name = mysql().Parking_name;
    QString createsql_check= QStringLiteral("select P_id from parking where P_name = '%1'; ").arg(park_name);
    QSqlQuery query =mysql().execute(createsql_check);
    if(!query.size()) //如果不存在此数据
    {
        qDebug()<<"Parking NUll";
        QString name = mysql().Parking_name;
        int all_count = mysql().parking_count;
        int now_count = 0;
        int park_reserve = 0;
        int p_fee = mysql().p_fee;
        //将停车场数据插入进入
        QSqlQuery qp;
        qp.prepare("insert into PARKING(P_name,P_now_count,P_reserve_count,P_all_count,P_fee) values(:name,:now_count,:P_reserve_count,:all_count,:p_fee);");
        qp.bindValue(":name",name);
        qp.bindValue(":now_count",now_count);
        qp.bindValue(":P_reserve_count",park_reserve);
        qp.bindValue(":all_count",all_count);
        qp.bindValue(":p_fee",p_fee);
        if(qp.exec())
        {
            qDebug()<<"Parking Initative successful";
        }
        else {
            qDebug()<<name<<all_count<<now_count<<p_fee;
            qDebug()<<"Parking Initative Error";
        }
    }else
    {
        qDebug()<<"Parking not NULL";
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
//实现停车位现有车位减+1
void mysql::parking_acc()
{
    QString location = mysql().Parking_name;
    QString createsql_Parking = QStringLiteral("UPDATE parking SET P_now_count = P_now_count + 1 WHERE P_name = '%1';").arg(location);

    if(mysql().execute_bool(createsql_Parking))
    {
        parking_now_count++;
        qDebug()<<"Parking Acc successful";
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
//预定车位表
void mysql::parking_reserve()
{
    QString createsql_reserve = QString("CREATE TABLE IF NOT EXISTS reservations("
                    "id INT PRIMARY KEY AUTO_INCREMENT, "
                   " license_plate VARCHAR ( 10 ) NOT NULL, "
                    "P_name VARCHAR ( 255 ) NOT NULL,"
                     "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                     "UNIQUE INDEX(license_plate)"
                    ");"
          );

    if(!execute_bool(createsql_reserve)) //执行创建表格语句
    {
        qDebug()<<"table Park reserve create error";
    }
    else
    {
        qDebug()<<"table Park reserve create success";
    }
    //建立触发器，如果车库预定表数量+1，触发器也自动加1，预定表新插入数据只有30分钟生存周期，会引发触发器，车库预定表数量-1
    QSqlQuery query;
    QStringList sqlStatements; // 存储多个SQL语句
    sqlStatements << "SET GLOBAL event_scheduler = ON;"; // 添加时间调度器
    sqlStatements << "CREATE EVENT IF NOT EXISTS clean_reservations ON SCHEDULE EVERY 30 MINUTE DO DELETE FROM reservations WHERE TIMESTAMPDIFF(MINUTE, created_at, NOW()) > 30;";  //创建清理 任务
    sqlStatements << QString("CREATE TRIGGER update_reserve_count AFTER INSERT ON reservations FOR EACH ROW BEGIN UPDATE PARKING SET P_reserve_count = P_reserve_count + 1 WHERE P_name = '%1'; END;").arg(Parking_name); //创建触发器，添加自动+1
    sqlStatements << QString("CREATE TRIGGER update_reserve_count2 AFTER DELETE ON reservations FOR EACH ROW BEGIN UPDATE PARKING SET P_reserve_count = P_reserve_count -1 where P_name = '%1' ;END;").arg(Parking_name); //创建触发器，减小后减一
    for (QStringList::iterator it = sqlStatements.begin(); it != sqlStatements.end(); it++) {
        if (!query.exec(*it)) { // 执行语句
            qDebug() << "触发器 executing SQL statement: " << query.lastError();
        }
    }
}

