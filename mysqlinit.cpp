#include "mysqlinit.h"

MySQLInit::MySQLInit(QObject *parent) : QObject(parent)
{

    mysqlc = new mysql;
    qDebug()<<"Init connection successful!";

}

MySQLInit::~MySQLInit()
{
    delete mysqlc; //释放申请的数据连接
}
void MySQLInit::create_user()
{
    QString createsql = QString("CREATE TABLE IF NOT EXISTS USER ( "
                                 "id INT PRIMARY KEY AUTO_INCREMENT, "
                                 "username VARCHAR(255) UNIQUE NOT NULL, "
                                 "password VARCHAR(255) NOT NULL, "
                                 "telephone VARCHAR(11) NOT NULL, "
                                 "truename VARCHAR(255) NOT NULL"
                                 ");");


    if(!mysqlc->execute(createsql).exec()) //执行创建表格语句
    {
        qDebug()<<"table create error";
    }
    else
    {
        qDebug()<<"table create success";
    }
}

void MySQLInit::create_car()
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

    if(!mysqlc->execute(createsql).exec()) //执行创建表格语句
    {
        qDebug()<<"table CAR create error";
    }
    else
    {
        qDebug()<<"table CAR create success";
    }
}

void MySQLInit::create_parking()
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

    if(!mysqlc->execute_bool(createsql)) //执行创建表格语句
    {
        qDebug()<<"table Parking create error";
    }
    else
    {
        qDebug()<<"table Parking create success";
    }
}

void MySQLInit::Parking_init(QString Parking_name)
{
    if(mysqlc->Parking_is_NULL(Parking_name)) //如果不存在此数据
    {
        qDebug()<<"Parking is NUll,creating";
        QString name = Parking_name;
        int all_count = mysqlc->parking_count;
        int now_count = 0;
        int park_reserve = 0;
        int p_fee = mysqlc->p_fee;
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
            //向用户输出错误信息
        }
    }else
    {
        qDebug()<<"Parking not NULL";
    }
}

void MySQLInit::parking_reserve()
{
    QString createsql_reserve = QString("CREATE TABLE IF NOT EXISTS reservations("
                    "id INT PRIMARY KEY AUTO_INCREMENT, "
                   " license_plate VARCHAR ( 10 ) NOT NULL, "
                    "P_name VARCHAR ( 255 ) NOT NULL,"
                     "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                     "UNIQUE INDEX(license_plate)"
                    ");"
          );

    if(!mysqlc->execute_bool(createsql_reserve)) //执行创建表格语句
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
    sqlStatements << "CREATE EVENT IF NOT EXISTS clean_reservations ON SCHEDULE EVERY 1 MINUTE DO DELETE FROM reservations WHERE TIMESTAMPDIFF(MINUTE, created_at, NOW()) > 30;";  //创建清理 任务
    sqlStatements << QString("CREATE TRIGGER update_reserve_count AFTER INSERT ON reservations FOR EACH ROW BEGIN UPDATE PARKING SET P_reserve_count = P_reserve_count + 1 WHERE P_name = '%1'; END;").arg(mysqlc->Parking_name); //创建触发器，添加自动+1
    sqlStatements << QString("CREATE TRIGGER update_reserve_count2 AFTER DELETE ON reservations FOR EACH ROW BEGIN UPDATE PARKING SET P_reserve_count = P_reserve_count -1 where P_name = '%1' ;END;").arg(mysqlc->Parking_name); //创建触发器，减小后减一
    for (QStringList::iterator it = sqlStatements.begin(); it != sqlStatements.end(); it++) {
        if (!query.exec(*it)) { // 执行语句
            qDebug() << "触发器 executing SQL statement: " << query.lastError();
        }
    }
}
