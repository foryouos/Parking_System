#ifndef CAR_H
#define CAR_H

#include <QWidget>

namespace Ui {
class Car;
}

class Car : public QWidget
{
    Q_OBJECT

public:
    explicit Car(QWidget *parent = nullptr);
    ~Car();

private:
    Ui::Car *ui;
};

#endif // CAR_H
