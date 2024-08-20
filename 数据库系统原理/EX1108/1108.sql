-- id name score
-- 2251435 李明浩 81.2
-- 2210465 赵毅斌 91.3
-- 2332133 刘孔阳 56.3
-- 2231435 王亚伟 73.2
-- 1950723 孙鹏翼 89.2

insert into Scores values(2251435, '李明浩', 81.2);
insert into Scores values(2210465, '赵毅斌', 91.3);
insert into Scores values(2332133, '刘孔阳', 56.3);
insert into Scores values(2231435, '王亚伟', 73.2);
insert into Scores values(1950723, '孙鹏翼', 89.2);

select * from Scores;

update Scores set score = 91.3 where id=2251435;
update Scores set score = 
