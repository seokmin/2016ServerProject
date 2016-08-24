
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
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;

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
