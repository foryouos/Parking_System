#ifndef INITFILE_H
#define INITFILE_H

#include <QWidget>
#include "mysql.h"
#include <QJsonDocument>  //保存JSON数据
#include "login.h"
#include "mysqlinit.h"
namespace Ui {
    class initFile;
}
//初始化数据库所需文件信息
class initFile : public QWidget
{
    Q_OBJECT

public:
    explicit initFile(QWidget *parent = nullptr);
    ~initFile();
    struct InitMySqlFileList
    {
        QString IP;
        int PORT;
        QString database;
        QString account;
        QString password;
        QString park_name;
        int P_fee;
        int P_count;
    };

signals:
    void starting(mysql mysql_c);
protected:

private slots:
    void on_submit_init_clicked(); //提交

    void on_cancel_submit_clicked(); //取消提交
    void Json_Save(struct InitMySqlFileList* list);
    bool File_MySQLInit();
private:
    Ui::initFile *ui;
    struct InitMySqlFileList* list;
    mysql *mysql_c; //数据库初始化所需要文件
    MySQLInit *Init;

};

#endif // INITFILE_H
