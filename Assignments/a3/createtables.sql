DROP TABLE PackageContent; 
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