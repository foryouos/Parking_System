-- 创建数据库
CREATE UPDATE Park;-- 创建表格停车场
-- 为了优化经纬度存储精度，经其设为如下DIOUBLE(11.8)
CREATE TABLE PLocation (
	L_id INT ( 20 ) PRIMARY KEY COMMENT '停车场id',
	L_name VARCHAR ( 255 ) COMMENT '停车场名称',
	L_longitude DOUBLE ( 11, 8 ) COMMENT '停车场所在经度',
	L_latitude DOUBLE ( 10, 8 ) COMMENT '停车场所在纬度',
	L_nowcount INT ( 20 ) COMMENT '停车场现有停车量',
	L_count INT ( 20 ) COMMENT '停车场总停车位',
	L_price DOUBLE COMMENT '停车场每小时收费' 
);-- 删除数据库表格
DROP TABLE PLocation;-- 描述数据库信息
DESC PLocation;
SHOW CREATE TABLE PLocation;-- 创建Car表
CREATE TABLE Car (
	C_id VARCHAR ( 255 ) PRIMARY KEY COMMENT "车牌号",
	C_img VARCHAR ( 255 ) COMMENT "车辆图片",
	P_locm INT ( 20 ) COMMENT "车位号",
	L_id INT ( 20 ) COMMENT "停车场外键",
	FOREIGN KEY ( L_id ) REFERENCES PLocation ( L_id ) 
);
DESC Car;-- 创建表格车位号
CREATE TABLE Pnum (
	P_locm INT ( 20 ) PRIMARY KEY COMMENT '车位号主键',
	L_id INT ( 20 ) COMMENT '隶属停车场外键',
	P_longitude DOUBLE ( 11, 8 ) COMMENT '车位所在经度',
	P_latitude DOUBLE ( 10, 8 ) COMMENT '车位所在纬度',
	p_height INT ( 20 ) COMMENT '车位海拔',
	FOREIGN KEY ( L_id ) REFERENCES Plocation ( L_id ) 
);
DESC Pnum;-- 为Car表添加外键
ALTER TABLE Car ADD FOREIGN KEY ( P_locm ) REFERENCES Pnum ( P_locm );-- 创建时间库
CREATE TABLE Ctime (
	C_id VARCHAR ( 255 ) COMMENT '车牌外键',
	L_id INT ( 20 ) COMMENT '所在停车场外键',
	C_enter DATETIME COMMENT '车辆进入停车场时间',
	C_leave DATETIME COMMENT '车辆离开停车场时间',
	FOREIGN KEY ( C_id ) REFERENCES Car ( C_id ),
	FOREIGN KEY ( L_id ) REFERENCES Plocation ( L_id ) 
);
DESC Ctime;