#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QSqlDatabase>  //添加数据库模块
#include <QSqlQuery> //数据库执行语句
#include <QMessageBox> //数据库执行语句
#include <QDebug>
#include "mysql.h" //引入mysql函数
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



//加密函数库
QString encryptPassword(QString password);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_pushButton_2_clicked();
    void on_return_2_clicked();


private:
    Ui::MainWindow *ui;
    mysql mysql_c;
};
#endif // MAINWINDOW_H
