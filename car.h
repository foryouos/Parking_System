#ifndef CAR_H
#define CAR_H

#include <QWidget>
#include "mysql.h"
namespace Ui {
class Car;
}

class Car : public QWidget
{
    Q_OBJECT

public:
    explicit Car(QWidget *parent = nullptr);
    ~Car();
    //切换函数
    void SwitchPage();

private slots:


    void on_CtrolButton_clicked();

private:
    Ui::Car *ui;
};

#endif // CAR_H
