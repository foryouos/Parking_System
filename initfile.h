#ifndef INITFILE_H
#define INITFILE_H

#include <QWidget>
#include "mysql.h"
#include <QJsonDocument>  //保存JSON数据
#include "login.h"
namespace Ui {
class initFile;
}

class initFile : public QWidget
{
    Q_OBJECT

public:
    explicit initFile(QWidget *parent = nullptr);
    ~initFile();

signals:
    void starting(mysql mysql_c);

private slots:
    void on_submit_init_clicked();

    void on_cancel_submit_clicked();

private:
    Ui::initFile *ui;

};

#endif // INITFILE_H
