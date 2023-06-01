#ifndef LOGINN_H
#define LOGINN_H

#include <QMainWindow>

namespace Ui {
class loginN;
}

class loginN : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginN(QWidget *parent = nullptr);
    ~loginN();

private:
    Ui::loginN *ui;
};

#endif // LOGINN_H
