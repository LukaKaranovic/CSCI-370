-- @file createdatabase.sql
-- @author Luka Karanovic, 665778833, S25N01, CSCI 370, VIU
-- @version 1.0.0
-- @date March 10, 2025

-- TABLE CREATION SECTION

DROP TABLE PackageContent;
DROP TABLE CurrentOrders;
DROP TABLE Orders;
DROP TABLE SupplyRecords;
DROP TABLE FoodItems;
DROP TABLE Producers;
DROP TABLE MemberAccounts;
DROP TABLE Packages;
DROP TABLE Batches;


CREATE TABLE MemberAccounts (
    accnum CHAR(6) PRIMARY KEY,
    contactName VARCHAR(80),
    email VARCHAR(100),
    deliveryAddress VARCHAR(200),
    signUpDate DATE,
    activationDate Date,
    status VARCHAR(10) default 'Active' CHECK (status IN ('Active', 'Inactive')),
    CHECK (activationDate >= signUpDate)
);

CREATE TABLE Producers (
    accnum CHAR(6) PRIMARY KEY REFERENCES MemberAccounts,
    farmAddress VARCHAR(200)
);

CREATE TABLE FoodItems (
    productCode CHAR(4) PRIMARY KEY,
    name VARCHAR(80),
    unit VARCHAR(10) CHECK (unit IN ('gram', 'quantity'))
);

CREATE TABLE SupplyRecords (
    accnum CHAR(6) REFERENCES Producers,
    productCode CHAR(4) REFERENCES FoodItems,
    supplyDate DATE,
    quantity NUMBER(7,0),
    PRIMARY KEY (accnum, productCode, supplyDate)
);

CREATE TABLE Batches (
    barcode CHAR(8) PRIMARY KEY,
    packingTime DATE,
    sizeType CHAR(1) CHECK (sizeType IN ('S', 'M', 'L'))
);

CREATE TABLE Packages (
    barcode CHAR(8) REFERENCES Batches,
    pNum  NUMBER(2,0) CHECK (pNum > 0),
    -- satisfyOrder CHAR(5) REFERENCES Orders,
    PRIMARY KEY (barcode, pNum)
);

CREATE TABLE Orders (
    orderNum CHAR(5) PRIMARY KEY,
    placedBy CHAR(6) REFERENCES MemberAccounts,
    placeTime DATE,
    packageType CHAR(1) CHECK (packageType IN ('S', 'M', 'L')),
    expectDate DATE,
    deliverTime DATE,
    receivedBarcode CHAR(8),
    receivedPNum NUMBER(2,0),
    FOREIGN KEY (receivedBarcode, receivedPNum) REFERENCES Packages
);

CREATE TABLE PackageContent (
    barcode CHAR(8) REFERENCES Batches,
    productCode CHAR(4) REFERENCES FoodItems,
    quantity NUMBER(4, 0),
    PRIMARY KEY (barcode, productCode)
);

-- SAMPLE DATA INSERTION 

-- Insert sample data into MemberAccounts
INSERT INTO MemberAccounts (accnum, contactName, email, deliveryAddress, signUpDate, activationDate, status) VALUES
('000001', 'John Doe', 'john.doe@example.com', '123 Green St, Cityville', TO_DATE('10-Jan-19', 'DD-Mon-YY'), TO_DATE('10-Jan-19', 'DD-Mon-YY'), 'Active');
INSERT INTO MemberAccounts (accnum, contactName, email, deliveryAddress, signUpDate, activationDate, status) VALUES
('000002', 'Jane Smith', 'jane.smith@example.com', '456 Blue Rd, Nanaimo', TO_DATE('05-Feb-23', 'DD-Mon-YY'), TO_DATE('10-Feb-23', 'DD-Mon-YY'), 'Active');
INSERT INTO MemberAccounts (accnum, contactName, email, deliveryAddress, signUpDate, activationDate, status) VALUES
('000003', 'Luka Karanovic', 'luka.karanovic@viu.ca', '678 Red Ave, Nanaimo', TO_DATE('10-Jan-19', 'DD-Mon-YY'), TO_DATE('05-Mar-20', 'DD-Mon-YY'), 'Active');

-- Insert sample data into Producers
INSERT INTO Producers (accnum, farmAddress) VALUES
('000001', '789 Farm Ln, Ruraltown');
INSERT INTO Producers (accnum, farmAddress) VALUES
('000003', '321 Orchard Ave, Countryside');

-- Insert sample data into FoodItems
INSERT INTO FoodItems (productCode, name, unit) VALUES
('1001', 'Broccoli', 'quantity');
INSERT INTO FoodItems (productCode, name, unit) VALUES
('1002', 'Carrots', 'gram');

-- Insert sample data into SupplyRecords
INSERT INTO SupplyRecords (accnum, productCode, supplyDate, quantity) VALUES
('000001', '1002', TO_DATE('20-Jan-24', 'DD-Mon-YY'), 50);
INSERT INTO SupplyRecords (accnum, productCode, supplyDate, quantity) VALUES
('000001', '1001', TO_DATE('21-Jan-24', 'DD-Mon-YY'), 9500);
INSERT INTO SupplyRecords (accnum, productCode, supplyDate, quantity) VALUES
('000001', '1001', TO_DATE('22-Jan-24', 'DD-Mon-YY'), 1000);
INSERT INTO SupplyRecords (accnum, productCode, supplyDate, quantity) VALUES
('000003', '1001', TO_DATE('23-Jan-24', 'DD-Mon-YY'), 2499);
INSERT INTO SupplyRecords (accnum, productCode, supplyDate, quantity) VALUES
('000003', '1001', TO_DATE('25-Jan-24', 'DD-Mon-YY'), 7500);

-- Insert sample data into Batches
INSERT INTO Batches (barcode, packingTime, sizeType) VALUES
('B0000001', TO_DATE('01-Feb-24', 'DD-Mon-YY'), 'M');
INSERT INTO Batches (barcode, packingTime, sizeType) VALUES
('B0000002', TO_DATE('02-Feb-24', 'DD-Mon-YY'), 'L');

-- Insert sample data into Packages
INSERT INTO Packages (barcode, pNum) VALUES
('B0000001', 1);
INSERT INTO Packages (barcode, pNum) VALUES
('B0000001', 2);
INSERT INTO Packages (barcode, pNum) VALUES
('B0000001', 3);
INSERT INTO Packages (barcode, pNum) VALUES
('B0000002', 1);
INSERT INTO Packages (barcode, pNum) VALUES
('B0000002', 2);

-- Insert sample data into Orders
INSERT INTO Orders (orderNum, placedBy, placeTime, packageType, expectDate, deliverTime, receivedBarcode, receivedPNum) VALUES
('10001', '000002', TO_DATE('05-Feb-24', 'DD-Mon-YY'), 'L', TO_DATE('1-Mar-25', 'DD-Mon-YY'), TO_DATE('14-Feb-24', 'DD-Mon-YY'), 'B0000001', 3);
INSERT INTO Orders (orderNum, placedBy, placeTime, packageType, expectDate, deliverTime, receivedBarcode, receivedPNum) VALUES
('10002', '000002', TO_DATE('06-Feb-24', 'DD-Mon-YY'), 'S', TO_DATE('1-Mar-25', 'DD-Mon-YY'), TO_DATE('11-Nov-24', 'DD-Mon-YY'), 'B0000001', 2);
INSERT INTO Orders (orderNum, placedBy, placeTime, packageType, expectDate, deliverTime, receivedBarcode, receivedPNum) VALUES
('10003', '000002', TO_DATE('07-Feb-23', 'DD-Mon-YY'), 'M', TO_DATE('12-Nov-24', 'DD-Mon-YY'), NULL, 'B0000002', 1);
INSERT INTO Orders (orderNum, placedBy, placeTime, packageType, expectDate, deliverTime, receivedBarcode, receivedPNum) VALUES
('10004', '000002', TO_DATE('09-Feb-24', 'DD-Mon-YY'), 'L', TO_DATE('15-Nov-24', 'DD-Mon-YY'), NULL, 'B0000002', 2);

-- Insert sample data into PackageContent
INSERT INTO PackageContent (barcode, productCode, quantity) VALUES
('B0000001', '1001', 5);
INSERT INTO PackageContent (barcode, productCode, quantity) VALUES
('B0000002', '1002', 500);