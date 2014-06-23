CREATE TABLE `Charger` (
  `ID` int(16) NOT NULL AUTO_INCREMENT,
  `UID` char(32) DEFAULT NULL,
  `CustomerID` char(32) DEFAULT NULL,
  `State` enum('in','out') DEFAULT NULL,
  `Created` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`)
)
