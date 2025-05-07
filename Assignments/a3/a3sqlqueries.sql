-- 1. List each member's name, email and delivery address;
select contactName, email, deliveryAddress
from MemberAccounts;

-- 2. List each producer's name, delivery address and farm address;
select M.contactName, M.deliveryAddress, P.farmAddress
from MemberAccounts M join Producers P on M.accnum = P.accnum;

-- 3. For each member whose delivery address contains "Nanaimo" and signed up to the program before March 15th, 2020,
-- list the member's contact name, delivery address and current status.
select contactName, deliveryAddress, status
from MemberAccounts
where deliveryAddress like '%Nanaimo%' and signUpDate < '15-MAR-20';

-- 4. For each unsatisfied order (i.e., their delivery time is still null) placed by any food producer, 
-- list the order's place time, expected delivery time, and the contact name of the member who placed the order.
select O.placeTime, O.expectDate, M.contactName
from Orders O join Producers P on O.placedBy = P.accnum
    join MemberAccounts M on M.accnum = P.accnum
where O.deliverTime is null;

-- 5. For each member who placed at least one order whose expected delivery time is in November 2024, 
-- list that member's account number, contact name and delivery address. There shouldn't be duplicates in the result.
select distinct M.accnum, M.contactName, M.deliveryAddress
from MemberAccounts M join Orders O on O.placedBy = M.accnum
where O.expectDate >= '01-NOV-24' and O.expectDate <= '30-NOV-24';