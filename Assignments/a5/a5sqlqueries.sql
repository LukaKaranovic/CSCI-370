-- 1. The Food Sharing Program wants to know how many active and inactive members it has respectively. The result from your query should be easily understandable.

select status, count(*) as amount
from MemberAccounts
group by status;

-- 2. For each member who signed up to the Food Sharing Program the earliest, list their contact name, email and current status.
    -- I assume this means to list the member(s) with the earliest signUpDate.

select contactName, email, status
from MemberAccounts
where signUpDate = (select min(signUpDate) from MemberAccounts);

-- 3. List the contact name, email, and farm address of each major broccoli producer of 2024. The "major broccoli producer of 2024" is defined as the producer 
-- who supplied at least 10,000 standard units of broccoli (whose name is stored as 'Broccoli' in the FoodItems table) in total in 2024.

select M.contactName, M.email, P.farmAddress
from MemberAccounts M natural join Producers P
where accnum in (select S.accnum
                from SupplyRecords S join FoodItems F on S.productCode = F.productCode
                where F.name = 'Broccoli' and S.supplyDate >= '01-Jan-24' and S.supplyDate <= '31-Dec-24'
                group by S.accnum
                having sum(S.quantity) >= 10000);

-- 4. For each active member (whose status is 'Active'), list their contact name, email, and the total number of orders they've placed. If an active member has never placed any order, 
-- their contact name and email should still appear in the result, and their total number of placed orders should be listed as 0. Sort the final result in descending order of the total 
-- number orders placed by these active members, then in ascending order of their contact names.

select M.contactName, M.email, count(O.orderNum) as totalOrders
from MemberAccounts M left join Orders O on M.accnum = O.placedBy
where status = 'Active'
group by M.accnum, M.contactName, M.email
order by totalOrders desc, M.contactName asc;

-- 5. The Food Sharing Program wants to know which sized package is the most popular one in 2024. That is, which sized package (package type) was ordered the most in 2024.
    -- I assume this means to list all packageTypes that are most popular in 2024 (so multiple if there are ties):
select packageType
from Orders
where placeTime >= '01-Jan-24' and placeTIme <= '31-Dec-24' 
group by packageType
having count(*) = (select max(package_count)
                  from (
                       select count(*) as package_count
                       from Orders
                       where placeTime >= '01-Jan-24' and placeTIme <= '31-Dec-24' 
                       group by packageType)
                  );