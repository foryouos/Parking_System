# 停车场管理系统
## 实现功能

- [x] 登陆功能
- [x] 注册功能
- [x] 账户信息管理增删改查
- [x] 停车信息查询
- [x] 车辆入库和车辆出库
- [x] 出库入库判断车牌信息判断
- [x] 微信小程序调用PHP数据库后端查询车场车位
- [x] 微信小程序预定系统
- [ ] 调用摄像头和视频
- [ ] 车牌识别
- [ ] 剩余车位预定可视化展示(实时同步小程序)
- [ ] 多线程连接池
- [ ] 优化数据库和QT多次调用的TCP开销

## UI展示
![前端页面](https://i.imgtg.com/2023/04/13/8RJGj.jpg "QT前端页面")
![微信小程序UI](https://i.imgtg.com/2023/04/13/8RysX.jpg "微信小程序UI")

## 文件解释
* 查询订阅小程序存放小程序源码(需要修改里面调用IP)
* 查询订阅小程序后端PHP(存放的PHP供小程序调用的后端代码)
* 论文部分(存储开发文档,以及和本项目相关的书籍和论文,老师要求的是完善版本,数据库文件也在里面)
* 首页的.pro是QT的运行文件
* images是QT文件调用的图片文件夹
## 环境:
### 客户端
* QT5.12.1
* C++11
* MySql 8.0.27
* OpenCV 3.2.0

### 微信小程序
#### 后台
* CentOS 7.6.1810 x86_64
* Nginx 1.18.0
* PHP-5.6.40
#### 前端
* 微信开发者工具1.06

## 环境配置
### QT换将
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

### 微信小程序环境
* [微信小程序连接MySQL数据库](https://blog.csdn.net/qq_37666407/article/details/126025101)
* 由于小程序调用API需要备案以及https连接域名，所有呈现使用在线调试或者使用PC预览，可以不备案直接端口查看使用情况
* [微信小程序调取以及中文乱码解决](https://blog.csdn.net/weixin_45925755/article/details/130118621)

