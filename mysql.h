#ifndef MYSQL_H
#define MYSQL_H
#include <QSqlQuery> //数据库执行语句
#include <QMessageBox> //数据库执行语句
#include <QDebug>

class mysql
{
private:
    QSqlDatabase db; //链接的数据库

public:
    mysql(); //初始化连接数据库
    ~mysql();// 析构语句
    void create_user(); //创建管理员用户表格
    //可以执行QT中MySQL的增删改查和建表操作
    QSqlQuery execute(QString createsql);//执行mysql语句,参数为执行语句

    bool execute_bool(QString createsql);


    void mysql_close(); //关闭mysql数据库连接

    // 开始事务
    void beginTransaction();
    // 提交事务
    void commitTransaction();
    // 回滚事务
    void rollbackTransaction();

};

#endif // MYSQL_H
