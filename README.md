# 停车场管理系统
## 实现功能

- [x] `登陆`功能
- [x] `注册`功能
- [x] 账户`信息管理`增删改查
- [x] 停车`信息查询`
- [x] 车辆`入库`和车辆`出库`
- [x] 出库入库判断车牌信息判断
- [x] 微信小程序调用`PHP`数据库后端查询车场车位
- [x] 微信小程序预定系统
- [x] 调用`摄像头`和`视频`
- [x] `车牌识别`
- [x] 剩余车位预定可视化展示(实时同步小程序)
- [x]  `优化EasyPR`过时的style，适配Qt5.12.1，C++11
- [x] 前端多线程连接池
- [ ] Linux后端优化数据库和QT多次调用的TCP开销

## UI展示
![整体页面](https://i.imgtg.com/2023/04/14/86qAB.png "整体页面")
![前端页面](https://i.imgtg.com/2023/04/13/8RJGj.jpg "QT前端页面")
![微信小程序UI](https://i.imgtg.com/2023/04/13/8RysX.jpg "微信小程序UI")

## 文件解释
* 查询订阅小程序存放小程序源码(需要`修改`里面调用`IP`)
* 查询订阅小程序后端PHP(存放的`PHP`供小程序`调用`的后端代码)
* 论文部分(存储`开发文档`,以及和本项目相关的`书籍`和`论文`,老师要求的是完善版本,数据库文件也在里面)
* 首页的`.pro`是`QT`的运行文件
* `images`是QT文件调用的图片文件夹
## 环境:

阅读详细[环境配置](./environment.md)

### 客户端

* `QT5.12.1`
* `C++11`
* `MySql 8.0.27`
* OpenCV 3.2.0
* 安装K-Lite_Codec_Pack_1676_Standard解析视频\(安装全部默认选项即可)

### 微信小程序
#### 后台

* `CentOS 7.6.1810 x86_64`
* ``Nginx 1.18.0`
* `PHP-5.6.40`

#### 前端

* 微信开发者工具``1.06`

## 环境配置
### QT换将
* MySQL环境配置
> ​	由于直接将在QT中引用MySQL`并不能`直接使用，所以需要将MySQL的动态链接库存放到QT的bin目录下，才可以调用MySQL。QT默认使用`C++11`编译

```
CONFIG += c++11
```

> 1,在QT所创建项目中的pro文件中添加sql

```sql
QT       += core gui sql
```
> 2,将MySQL中lib库里面的`libmysql.dll`文件和`libmysql.lib`文件赋值粘贴到`QT5.12.1`的`msvc2017_64`文件夹bin文件内

![mysql环境](https://i.imgtg.com/2023/04/04/22u3S.png "mysql环境")

* [OpenCV环境配置](https://blog.csdn.net/Mr_robot_strange/article/details/110677323 "openCV环境配置")
* [EasyPr换将配置](https://www.jianshu.com/p/22617e584f28 "easyPR环境配置")



### 微信小程序

* [微信小程序`连接MySQL`数据库](https://blog.csdn.net/qq_37666407/article/details/126025101)
* 由于小程序调用`API`需要`备案`以及`https`连接`域名`，所有呈现使用在线调试或者使用PC预览，可以不备案直接端口查看使用情况
* [微信小程序调取以及`中文乱码`解决](https://blog.csdn.net/weixin_45925755/article/details/130118621)
* [微信小程序更改`TabBar`](https://blog.csdn.net/SoulNone/article/details/127933751)

