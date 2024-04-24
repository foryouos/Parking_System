-- 创建数据库
CREATE DATABASE car;

use car;
-- 创建管理员账户表
CREATE TABLE
IF
	NOT EXISTS USER (
		id INT PRIMARY KEY AUTO_INCREMENT,
		username VARCHAR ( 255 ) UNIQUE NOT NULL,
		PASSWORD VARCHAR ( 255 ) NOT NULL,
		telephone VARCHAR ( 11 ) NOT NULL,
		truename VARCHAR ( 255 ) NOT NULL 
	);

-- 创建停车场信息表
CREATE TABLE
IF
	NOT EXISTS PARKING (
		P_id INT NOT NULL AUTO_INCREMENT,
		P_name VARCHAR ( 255 ) NOT NULL,
		P_reserve_count INT,
		P_now_count INT,
		P_all_count INT,
		P_fee DECIMAL ( 10, 2 ),
		PRIMARY KEY ( P_id ) 
	);

-- 创建汽车表
CREATE TABLE
IF
	NOT EXISTS CAR (
		C_id INT NOT NULL AUTO_INCREMENT,
		license_plate VARCHAR ( 20 ) NOT NULL,
		check_in_time DATETIME NOT NULL,
		check_out_time DATETIME DEFAULT NULL,
		fee DECIMAL ( 10, 2 ) DEFAULT NULL,
		location VARCHAR ( 20 ) NOT NULL,
		PRIMARY KEY ( id ),
	INDEX ( license_plate ) 
	);
	
	CREATE TABLE IF NOT EXISTS reservations 
	( 
	R_id INT PRIMARY KEY AUTO_INCREMENT, 
	license_plate VARCHAR ( 10 ) NOT NULL, 
	P_name VARCHAR ( 255 ) NOT NULL,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    UNIQUE INDEX(license_plate)
	);