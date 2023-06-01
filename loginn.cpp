#include "loginn.h"
#include "ui_loginn.h"

loginN::loginN(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginN)
{
    ui->setupUi(this);
}

loginN::~loginN()
{
    delete ui;
}
