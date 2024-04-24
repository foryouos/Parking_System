-- 预约表
CREATE TABLE IF NOT EXISTS reservations 
	( 
	id INT PRIMARY KEY AUTO_INCREMENT, 
	license_plate VARCHAR ( 10 )  NOT NULL, 
	P_name VARCHAR ( 255 ) NOT NULL,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	UNIQUE INDEX(license_plate)
	);
	
-- 	DROP TABLE reservations;
	
	DESC reservations;
	
	SELECT * FROM reservations;
	
	DROP TABLE parking;
	
	CREATE TABLE IF NOT EXISTS PARKING(P_id INT NOT NULL AUTO_INCREMENT,
                                    P_name VARCHAR(255) NOT NULL,
                                    P_reserve_count INT,
                                    P_now_count INT,
                                    P_all_count INT,
                                    P_fee DECIMAL(10, 2),
                                    PRIMARY KEY (P_id),
                                    UNIQUE INDEX(P_name)
                                );

DESC parking;

-- 插入数据，做准备
INSERT INTO parking VALUES(1,"新工停车场",0,0,300,15);
INSERT INTO parking VALUES(2,"河师大停车",22,0,400,15);
INSERT INTO parking VALUES(3,"胖东来停车",15,30,100,15);
INSERT INTO parking VALUES(4,"龙亭停车场",3,10,50,15);
INSERT INTO parking VALUES(5,"卫辉停车场",0,65,200,15);
INSERT INTO parking VALUES(6,"原阳停车场",7,78,300,15);
INSERT INTO parking VALUES(7,"红旗停车场",5,12,450,15);




-- 需要读取的数据
SELECT P_name , P_all_count-P_reserve_count-P_reserve_count AS surplus FROM parking;















