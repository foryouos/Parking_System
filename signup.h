#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include "mainwindow.h" //引入登陆窗口
namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_sure_clicked();

private:
    Ui::Signup *ui;
    mysql mysql_c;
};

#endif // SIGNUP_H
