-- Insert sample data into MemberAccounts
INSERT INTO MemberAccounts (accnum, contactName, email, deliveryAddress, signUpDate, activationDate, status) VALUES
('000001', 'John Doe', 'john.doe@example.com', '123 Green St, Cityville', TO_DATE('10-Jan-19', 'DD-Mon-YY'), TO_DATE('15-Jan-20', 'DD-Mon-YY'), 'Active');
INSERT INTO MemberAccounts (accnum, contactName, email, deliveryAddress, signUpDate, activationDate, status) VALUES
('000002', 'Jane Smith', 'jane.smith@example.com', '456 Blue Rd, Nanaimo', TO_DATE('05-Feb-23', 'DD-Mon-YY'), TO_DATE('10-Feb-23', 'DD-Mon-YY'), 'Inactive');
INSERT INTO MemberAccounts (accnum, contactName, email, deliveryAddress, signUpDate, activationDate, status) VALUES
('000003', 'Luka Karanovic', 'luka.karanovic@viu.ca', '678 Red Ave, Nanaimo', TO_DATE('05-Mar-20', 'DD-Mon-YY'), TO_DATE('10-Feb-23', 'DD-Mon-YY'), 'Active');

-- Insert sample data into Producers
INSERT INTO Producers (accnum, farmAddress) VALUES
('000001', '789 Farm Ln, Ruraltown');
INSERT INTO Producers (accnum, farmAddress) VALUES
('000003', '321 Orchard Ave, Countryside');

-- Insert sample data into FoodItems
INSERT INTO FoodItems (productCode, name, unit) VALUES
('1001', 'Apples', 'quantity');
INSERT INTO FoodItems (productCode, name, unit) VALUES
('1002', 'Carrots', 'gram');

-- Insert sample data into SupplyRecords
INSERT INTO SupplyRecords (accnum, productCode, supplyDate, quantity) VALUES
('000001', '1001', TO_DATE('20-Jan-24', 'DD-Mon-YY'), 50);
INSERT INTO SupplyRecords (accnum, productCode, supplyDate, quantity) VALUES
('000003', '1002', TO_DATE('21-Jan-24', 'DD-Mon-YY'), 1000);

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
('10001', '000001', TO_DATE('05-Feb-24', 'DD-Mon-YY'), 'M', TO_DATE('10-Feb-24', 'DD-Mon-YY'), TO_DATE('14-Feb-24', 'DD-Mon-YY'), 'B0000001', 3);
INSERT INTO Orders (orderNum, placedBy, placeTime, packageType, expectDate, deliverTime, receivedBarcode, receivedPNum) VALUES
('10002', '000002', TO_DATE('05-Feb-24', 'DD-Mon-YY'), 'S', TO_DATE('10-Nov-24', 'DD-Mon-YY'), TO_DATE('11-Nov-24', 'DD-Mon-YY'), 'B0000001', 2);
INSERT INTO Orders (orderNum, placedBy, placeTime, packageType, expectDate, deliverTime, receivedBarcode, receivedPNum) VALUES
('10003', '000003', TO_DATE('06-Feb-24', 'DD-Mon-YY'), 'L', TO_DATE('12-Nov-24', 'DD-Mon-YY'), NULL, 'B0000002', 1);
INSERT INTO Orders (orderNum, placedBy, placeTime, packageType, expectDate, deliverTime, receivedBarcode, receivedPNum) VALUES
('10004', '000003', TO_DATE('06-Feb-24', 'DD-Mon-YY'), 'L', TO_DATE('15-Nov-24', 'DD-Mon-YY'), NULL, 'B0000002', 2);

-- Insert sample data into PackageContent
INSERT INTO PackageContent (barcode, productCode, quantity) VALUES
('B0000001', '1001', 5);
INSERT INTO PackageContent (barcode, productCode, quantity) VALUES
('B0000002', '1002', 500);