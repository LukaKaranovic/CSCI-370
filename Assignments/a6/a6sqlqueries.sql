-- PURE PSJ SQL QUERIES (NO SUBQUERIES):

-- 1. List the contact name and email of each inactive member (whose status is 'Inactive') who has placed at least one order.

select distinct M.contactName, M.email
from MemberAccounts M join Orders O on M.accnum = O.placedBy
where M.status = 'Inactive';

-- 2. List the contact name and email of each active member (whose status is 'Active') who has never placed any order.

select M.contactName, M.email
from MemberAccounts M left join Orders O on M.accnum = O.placedBy
where status = 'Active' and O.placedBy is null;

-- SQL QUERIES THAT ARE ALSO IN DATALOG:

-- a. For each active member (whose status is 'Active'), list their contact name and email, and the order number and package type of their latest placed order 
-- (whose place time is the largest).

select M.contactName, M.email, O.orderNum, O.packageType
from MemberAccounts M join Orders O on M.accnum = O.placedBy
join (select placedBy, max(placeTime) as latestPlaceTime
    from Orders
    group by placedBy
    ) LatestOrders on O.placedBy = LatestOrders.placedBy and O.placeTime = LatestOrders.latestPlaceTime 
where M.status = 'Active';

-- b. List the contact name, email and farm address of each producer who is capable of supply EVERY food item. A producer is considered to be able to supply every 
-- food item if this producer had supplied every food item in the FoodItems table before.

select M.contactName, M.email, P.farmAddress
from MemberAccounts M join Producers P on M.accnum = P.accnum
where not exists (
                select productCode
                from FoodItems
                minus
                select productCode
                from SupplyRecords S
                where S.accnum = P.accnum
                );            

-- CURRENTORDERS TABLE MAINTENANCE QUERIES:

-- Step 1: delete all the data used in the previous delivery day from the table CurrentOrders;

delete
from CurrentOrders;

-- Step 2: gather all the relevant information of each order whose expected delivery date is today and insert them into CurrentOrders table. 
-- (Note that Oracle uses current_date for today's date.)

insert into CurrentOrders
select O.orderNum, O.placedBy, M.contactName, M.email, M.deliveryAddress, O.packageType
from Orders O join MemberAccounts M on O.placedBy = M.accnum
where O.expectDate = trunc(current_date);
-- I use trunc() just to get the date portion of current_date only, no time.
