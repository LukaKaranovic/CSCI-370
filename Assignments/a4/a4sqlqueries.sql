-- 1. For each loyal member who had placed at least one order, list the member's contact name and delivery address.
-- There shouldn't be any duplicates in the result.
-- Extra information: A loyal member is defined as someone who has never cancelled their membership with the Food Sharing Program. 
-- In other words, their status is still 'Active' and their sign up date and activation date remain the same.

select distinct M.contactName, M.deliveryAddress
from MemberAccounts M join Orders O on M.accnum = O.placedBy
where M.status = 'Active' and M.signUpDate = M.activationDate;

-- 2. For each order that received (and accepted presumably because the data is in the database now) a wrong sized package 
-- (i.e., the package size demanded by the order is different from the size of the package delivered), list the contact name and delivery address of 
-- the member who placed the order, and the delivered package's batch barcode and package size, and the delivery date.

select M.contactName, M.deliveryAddress, O.receivedBarcode, B.sizeType as receivedSize, O.deliverTime
from Orders O join MemberAccounts M on O.placedBy = M.accnum
    join Batches B on O.receivedBarcode = B.barcode
where O.packageType <> B.sizeType and O.receivedBarcode is not null;

-- 3. List the contact name, email and delivery address of each member whose status is 'Active' now, but who never placed any order in year 2024.
-- There shouldn't be any duplicates in the result.

select contactName, email, deliveryAddress
from MemberAccounts
where status = 'Active' and accnum not in (select placedBy
                                          from Orders
                                          where placeTime >= '01-Jan-24' and placeTime <= '31-Dec-24');

-- 4. List the contact name, email and farm address of each producer who supplied some food item that's not broccoli to the 
-- Food Sharing Program in 2024. Broccoli's name is stored as 'Broccoli' in FoodItems table. There shouldn't be any duplicates in the result.

select M.contactName, M.email, P.farmAddress
from Producers P natural join MemberAccounts M
where accnum in (select accnum
                from SupplyRecords natural join FoodItems
                where name <> 'Broccoli' and supplyDate >= '01-Jan-24' and supplyDate <= '31-Dec-24');

-- 5. List the contact name, email and farm address of each producer who never supplied any broccoli to the 
-- Food Sharing Program in 2024. Broccoli's name is stored as 'Broccoli' in FoodItems table.

select M.contactName, M.email, P.farmAddress
from Producers P natural join MemberAccounts M
where accnum not in (select accnum
                    from SupplyRecords natural join FoodItems
                    where name = 'Broccoli' and supplyDate >= '01-Jan-24' and supplyDate <= '31-Dec-24');