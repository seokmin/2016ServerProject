
CREATE TABLE `auth` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(45) NOT NULL,
  `authToken` varchar(45) NOT NULL,
  `timestamp` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `channel` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `ip` varchar(45) NOT NULL,
  `port` int(11) DEFAULT NULL,
  `r` int(11) DEFAULT NULL,
  `g` int(11) DEFAULT NULL,
  `b` int(11) DEFAULT NULL,
  `minBet` int(11) DEFAULT NULL,
  `maxBet` int(11) DEFAULT NULL,
  `curNum` int(11) NOT NULL,
  `maxNum` int(11) NOT NULL,
  `timestamp` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;

CREATE TABLE `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(45) NOT NULL,
  `pw` varchar(255) NOT NULL,
  `pokemon` int(11) DEFAULT NULL,
  `chip` int(11) NOT NULL DEFAULT '500',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username_UNIQUE` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;


-- ----------------create stored procedure----------------------------
-- ----------------submit_server_status----------------------------
USE `jackblack`;
DROP procedure IF EXISTS `submit_server_status`;

DELIMITER $$
USE `jackblack`$$
CREATE DEFINER=`next`@`%` PROCEDURE `submit_server_status`(
	IN serverName VARCHAR(20),
    serverIP VARCHAR(16), serverPort INT,
    red INT, green INT, blue INT,
    minBetMoney INT, maxBetmoney INT,
    curUserNum INT, maxUserNum INT
    )
BEGIN
INSERT INTO channel
SET name = serverName,
	ip = serverIP, port = serverPort,
	r = red, g = green, b = blue,
	minBet = minBetMoney, maxBet = maxBetmoney,
	curNum = curUserNum , maxNum = maxUserNum, timestamp = NOW()
ON DUPLICATE KEY UPDATE
	name = serverName,
	ip = serverIP, port = serverPort,
	r = red, g = green, b = blue,
	minBet = minBetMoney, maxBet = maxBetmoney,
	curNum = curUserNum , maxNum = maxUserNum, timestamp = NOW();
SELECT id, timestamp FROM channel WHERE name = serverName;
END$$
DELIMITER ;


-- ---------------GetUserInfoByAuthToken-----------------------
USE `jackblack`;
DROP procedure IF EXISTS `GetUserInfoByAuthToken`;

DELIMITER $$
USE `jackblack`$$
CREATE DEFINER=`next`@`%` PROCEDURE `GetUserInfoByAuthToken`(IN aToken VARCHAR(45))
BEGIN
SELECT u.username, pokemon, chip, authToken FROM auth AS a JOIN user AS u ON a.username = u.username HAVING authToken = aToken;
END$$

DELIMITER ;


-- ---------------Calc_user_money---------------------
USE `jackblack`;
DROP procedure IF EXISTS `Calc_user_money`;

DELIMITER $$
USE `jackblack`$$
CREATE DEFINER=`next`@`%` PROCEDURE `Calc_user_money`(IN TargetName VARCHAR(45), deltaMoney INT)
BEGIN
	UPDATE user SET chip = chip + deltaMoney
    WHERE username LIKE TargetName;
    SELECT username, chip FROM user where username = TargetName;
END$$

DELIMITER ;


