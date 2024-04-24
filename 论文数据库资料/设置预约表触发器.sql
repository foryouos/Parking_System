SHOW TABLES;
DESC parking;

SELECT P_now_count,P_all_count FROM parking WHERE P_name = "新工停车场";

select * from car;
SELECT * FROM parking;

-- DROP TABLE reservations;
-- 1	新工停车场 0	4	500	30.00

-- DROP TABLE parking;


	
	

CREATE TABLE IF NOT EXISTS PARKING(P_id INT NOT NULL AUTO_INCREMENT,
                                    P_name VARCHAR(255) NOT NULL,
                                    P_reserve_count INT,
                                    P_now_count INT,
                                    P_all_count INT,
                                    P_fee DECIMAL(10, 2),
                                    PRIMARY KEY (P_id),
                                    UNIQUE INDEX(P_name)
                                );
																
INSERT INTO parking VALUES(1,"新工停车场",0,4,500,30.00)


-- 预约表
CREATE TABLE IF NOT EXISTS reservations 
	( 
	id INT PRIMARY KEY AUTO_INCREMENT, 
	license_plate VARCHAR ( 10 )  NOT NULL, 
	P_name VARCHAR ( 255 ) NOT NULL,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP
	);
	
	DESC reservations;
	
	ALTER TABLE reservations ADD UNIQUE INDEX(license_plate);
	
	-- 添加事件调度器
SET GLOBAL event_scheduler = ON;
	
	-- 创建清理任务
CREATE EVENT IF NOT EXISTS clean_reservations
ON SCHEDULE EVERY 30 MINUTE
DO
    DELETE FROM reservations WHERE TIMESTAMPDIFF(MINUTE, created_at, NOW()) > 30;
	
	
-- 触发器，当
DELIMITER //
CREATE TRIGGER update_reserve_count AFTER INSERT ON reservations
FOR EACH ROW 
BEGIN 
    UPDATE PARKING SET P_reserve_count = P_reserve_count + 1 WHERE P_name = "新工停车场"; 
END//
DELIMITER ;


DELIMITER //
CREATE TRIGGER IF NOT EXISTS update_reserve_count2 AFTER DELETE ON reservations
FOR EACH ROW 
BEGIN
    UPDATE PARKING SET P_reserve_count = P_reserve_count -1 where P_name = "新工停车场";
END//
DELIMITER ;
