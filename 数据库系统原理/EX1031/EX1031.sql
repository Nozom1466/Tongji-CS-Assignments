-- 1. create table
create table studentA (
  Sno varchar(9) primary key,
  Sname varchar(20),
  Ssex varchar(2),
  Sage smallint,
  Sdept varchar(20),
  constraint studentA_ul unique (Sname)
);

desc studentA;



-- 2. insert items
insert into studentA values ('200215121', 'Tom', 'm', 20, 'CS');
insert into studentA values ('200215122', 'Lily', 'f', 19, 'CS');
select * from studentA;



-- 3. cretae user 'masterA' with pwd "key"
create user masterA identified by "key";
grant create,
      create user,
      alter,
      drop on *.*
      to masterA
      with grant option;



-- 4. revoke privileges
revoke create user, alter, drop on *.* from masterA;


-- 6. 在masterA用户下尝试查询table studentA(注意使用sys.studentA)
select * from sys.studentA;


-- 7. 授予用户masterA对表studentA的查询、插入、修改等对象特权，并赋予其再授权的能力
grant select, insert, update, delete on studentA to masterA with grant option;


-- 8. 在masterA用户下查询sys.studentA(注意使用sys.studentA)
select * from sys.studentA;

-- 9. 在masterA用户下再插入一个元组的数据('200215123','Bob','m',21,'IS')，并在masterA用户下查询(注意使用sys.studentA)
insert into sys.studentA values ('200215123','Bob','m',21,'IS');
select * from sys.studentA;


-- 10.删除今天创建的masterA用户

