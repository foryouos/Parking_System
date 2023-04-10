#include "init.h"
#include "ui_init.h"

Init::Init(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Init)
{
    ui->setupUi(this);
}

Init::~Init()
{
    delete ui;
}
