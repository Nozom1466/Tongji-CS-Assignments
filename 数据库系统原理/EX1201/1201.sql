create database library_sys

use library_sys

----------------------------------------------------------
-- create tables
----------------------------------------------------------


create table Book (
    bno varchar(10) primary key,
    bname varchar(30),
    author varchar(30),
    price float
);

create table Student (
    sno varchar(10) primary key,
    sname varchar(30),
    grade varchar(5)
);

create table Borrow (
    sno varchar(10),
    bno varchar(10),
    rdate datetime,
    primary key(sno, bno),
    foreign key(sno) references Student(sno) on delete cascade,
    foreign key(bno) references Book(bno) on delete cascade
);

----------------------------------------------------------
-- insert vallues
----------------------------------------------------------

insert into Book values
    ('T1001','Java 程序设计','李新民',23.5),
    ('T1002','数据库原理及应用' ,'王珊',27),
    ('T1003','Java 高级编程','陈海',23.5),
    ('T1004','大学英语','张宁',18.5),
    ('T1005','C++程序设计','马品三',33.5),
    ('T1006','数据结构','刘子单',35);
insert into Student values
    ('K001','张三','大一'),
    ('K002','李四','大二'),
    ('K003','王五','大三'),
    ('K004','赵六','大四'),
    ('K005','刘七','大四');
insert into Borrow values
    ('K001','T1006','2022-10-9'),
    ('K001','T1001','2022-3-1'),
    ('K001','T1004','2022-5-7'),
    ('K002','T1002','2022-6-9'),
    ('K002','T1003','2022-12-5'),
    ('K002','T1001','2022-11-3'),
    ('K003','T1005','2022-9-4'),
    ('K004','T1002','2022-2-5');


----------------------------------------------------------
-- 查询书名中包含 “程序设计” 的图书信息，输出所有信息（包括书名、书号、作者、单价），并按照单价降序排列
----------------------------------------------------------

select bname as '书名', bno as '书号', author as '作者', price as '单价'
from Book 
where bname like '%程序设计%' 
order by price desc;


----------------------------------------------------------
-- 查询借阅了书名为”数据库原理及应用”的学生信息，输出该学生的学号、姓名和年级；
----------------------------------------------------------
select stu.sno as '学号', stu.sname as '姓名', stu.grade as '年级'
from Student as stu join Borrow as bor on stu.sno = bor.sno 
                    join Book as bok on bor.bno = bok.bno
where bok.bname = '数据库原理及应用';


----------------------------------------------------------
-- 统计每个学生借书信息，输出每个学生的学号、借书书名和还书日期；
----------------------------------------------------------
select stu.sno as '学号', bok.bname as '借书书名', bor.rdate as '还书日期'
from Borrow as bor join Book as bok on bor.bno = bok.bno
                   join Student as stu on bor.sno = stu.sno;


----------------------------------------------------------
-- 查询所有借阅已过期图书的信息，输出学生姓名、书名和还书日期；
----------------------------------------------------------
select stu.sname as '学生姓名', bok.bname as '书名', bor.rdate as '还书日期'
from Borrow as bor join Book as bok on bor.bno = bok.bno
                   join Student as stu on bor.sno = stu.sno
where bor.rdate < '2022-11-8';


----------------------------------------------------------
-- 查询没有借阅过书的学生信息，输出学生姓名和学号
----------------------------------------------------------
select stu.sname as '学生姓名', stu.sno as '学号'
from Student as stu left join Borrow as bor on stu.sno = bor.sno
where bor.sno is null;


----------------------------------------------------------
-- 查询借了"Java 程序设计"但没有借"数据库原理及应用"的读者信息，输出这些学生的学号，并按照学号升序排列；
----------------------------------------------------------
select stu.sno as '学号'
from Student as stu 
where exists (
    (
        select * 
        from Borrow as bor join Book as bok on bor.bno = bok.bno
        where bor.sno = stu.sno and bok.bname = 'Java 程序设计'
    )
    except 
    (
        select * 
        from Borrow as bor join Book as bok on bor.bno = bok.bno
        where bor.sno = stu.sno and bok.bname = '数据库原理及应用'
    )
)
order by stu.sno;


----------------------------------------------------------
-- 9. 创建一个过程，使之能够实现如下功能： 
--      a) 修改借阅表，增加字段”借阅状态”（字段名为”bstate”，数据类型
--         可自行定义），字段含义为表示图书的借阅状态是否已经过期； 
--      b) 并根据表中已有数据为该字段赋值（所赋的值与表定义时的数据类型保
--         持一致即可，比如可以定义已到期图书的”借阅状态”为 True，未到期
--         图书的”借阅状态”为 False），要求使用 if 语句进行条件判断；
----------------------------------------------------------

delimiter $$

create procedure update_bstate()
begin
    -- add field bstate
    alter table Borrow add column bstate boolean;

    -- update bstate field based on rdate field
    update Borrow 
    set bstate = if (rdate < '2022-11-08', false, true);

end$$

delimiter ;

call update_bstate();

select * from Borrow;


----------------------------------------------------------
-- 修改图书表，在 bname 列上增加唯一性索引 bname_index，并按 bname降序排列；
----------------------------------------------------------
alter table Book
add unique index bname_index (bname desc);

select * from Book;

show index from Book;

