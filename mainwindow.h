#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>  //添加数据库模块
#include <QSqlQuery> //数据库执行语句
#include <QMessageBox> //数据库执行语句
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//初始化数据库
void sqlite_Init();
//初始化MySQL
void mysql_Init();

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
};
#endif // MAINWINDOW_H
