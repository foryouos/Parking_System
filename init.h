#ifndef INIT_H
#define INIT_H

#include <QWidget>

namespace Ui {
class Init;
}

class Init : public QWidget
{
    Q_OBJECT

public:
    explicit Init(QWidget *parent = nullptr);
    ~Init();

private:
    Ui::Init *ui;
};

#endif // INIT_H
