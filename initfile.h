#ifndef INITFILE_H
#define INITFILE_H

#include <QWidget>
#include "mysql.h"
#include <QJsonDocument>  //保存JSON数据
#include "mainwindow.h"
namespace Ui {
class initFile;
}

class initFile : public QWidget
{
    Q_OBJECT

public:
    explicit initFile(QWidget *parent = nullptr);
    ~initFile();

private slots:
    void on_submit_init_clicked();

private:
    Ui::initFile *ui;
};

#endif // INITFILE_H
