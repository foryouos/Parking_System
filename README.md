# 停车场管理系统
## 环境:
* QT5.12.1
* C++11
* MySql 8.0.27
* OpenCV 3.2.0
## 环境配置

* MySQL环境配置
> ​	由于直接将在QT中引用MySQL并不能直接使用，所以需要将MySQL的动态链接库存放到QT的bin目录下，才可以调用MySQL。QT默认使用C++11编译

```
CONFIG += c++11
```

> 1,在QT所创建项目中的pro文件中添加sql

```sql
QT       += core gui sql
```
> 2,将MySQL中lib库里面的libmysql.dll文件和libmysql.lib文件赋值粘贴到QT5.12.1的msvc2017_64文件夹bin文件内

![mysql环境](https://i.imgtg.com/2023/04/04/22u3S.png "mysql环境")

## 实现功能

* 登陆功能
* 注册功能
* 车牌识别
* 车位可视化
* 停车信息查询
* 车辆入库和车辆出库
* 微信小程序调用查询车位