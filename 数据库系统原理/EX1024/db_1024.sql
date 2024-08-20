create table depts1 (
  no integer not null auto_increment,
  name varchar(30) not null,
  primary key (no)
);

create table students1 (
  no integer not null auto_increment,
  name varchar(20) not null,
  gender varchar(6) not null check (gender='Male' or gender='Female'),
  age integer not null,
  d_no integer not null,
  primary key (no),
  constraint st_c_1 foreign key (d_no) references depts1(no)
);

create table courses1 (
  no integer not null auto_increment,
  name varchar(20) not null,
  credit integer not null,
  d_no integer not null,
  primary key (no),
  constraint co_c_1 foreign key (d_no) references depts1(no)
);

create table scores1 (
  s_no integer not null auto_increment,
  c_no integer not null,
  score integer not null,
  constraint sc_c_1 foreign key (s_no) references students1(no),
  constraint sc_c_2 foreign key (c_no) references courses1(no)
);

insert into depts1 (no,name) values (1, 'Computer Science');
insert into depts1 (no,name) values (2, 'Mathematics');
insert into depts1 (no,name) values (3, 'Architecture');
insert into depts1 (no,name) values (4, 'Management');

insert into courses1 (no, name, credit, d_no) values (1, 'DataBase', 5, 1);
insert into courses1 (no, name, credit, d_no) values (2, 'Mathematics', 2, 2);
insert into courses1 (no, name, credit, d_no) values (3, 'Information System', 1, 4);
insert into courses1 (no, name, credit, d_no) values (4, 'Operating System', 6, 1);
insert into courses1 (no, name, credit, d_no) values (5, 'Data Structure', 4, 1);
insert into courses1 (no, name, credit, d_no) values (6, 'Data Processing', 2, 4);
insert into courses1 (no, name, credit, d_no) values (7, 'PASCAL', 3, 1);

insert into students1 (no, name, gender, age, d_no) values (200215120, 'Mike', 'Male', 21, 3);
insert into students1 (no, name, gender, age, d_no) values (200215121, 'Tom', 'Male', 20, 1);
insert into students1 (no, name, gender, age, d_no) values (200215122, 'Jerry', 'Female', 19, 1);
insert into students1 (no, name, gender, age, d_no) values (200215123, 'Alice', 'Female', 18, 2);
insert into students1 (no, name, gender, age, d_no) values (200215125, 'Bob', 'Male', 19, 3);

insert into scores1 (s_no, c_no, score) values(200215121, 1, 92);
insert into scores1 (s_no, c_no, score) values(200215121, 2, 85);
insert into scores1 (s_no, c_no, score) values(200215121, 3, 88);
insert into scores1 (s_no, c_no, score) values(200215122, 2, 90);
insert into scores1 (s_no, c_no, score) values(200215122, 3, 80);

/*
NO.1 查所有年龄在21岁以下的学生姓名及其年龄（使用比较运算符）
*/
select name, age
from students1
where age < 21;

/*
NO.2 查询选2号课程(c_no='2')且成绩在80--90的学生号。(BETWEEN  … AND …   )
*/
select s_no 
from scores1
where c_no = '2' and score between 80 and 90;

/*
NO.3 查姓名第二个字母是'e'的学生姓名
*/
select name 
from students1
where name like '_e%';

/*
NO.4  查询全体男NO.3 查姓名第二个字母是'e'的学生姓名学生的学号、系、年龄。结果按所在的系升序排列，同一系中的学生按年龄降序排列。
*/
select stu.no as student_ID, dept.name as department_name, stu.age as student_age
from students1 as stu inner join depts1 as dept on stu.d_no = dept.no
where stu.gender = 'Male'
order by dept.name asc, stu.age desc;

/*
NO.5  查询女学生的总人数和平均年龄.
*/
select COUNT(gender), AVG(age)
from students1 
where gender = 'Female';

/*	
NO.6  查询选修3号课程并及格【分数大于60】的学生的最高分数、最低分及总分。
*/
select max(sco.score), min(sco.score), sum(sco.score)
from students1 as stu inner join scores1 as sco on stu.no = sco.s_no
where sco.c_no = '3' and sco.score > 60;

/*
NO.7  向Score表中插入一条记录 200215123,1,72
*/
INSERT INTO scores1 (s_no, c_no, score) VALUES (200215123, 1, 72);

/*
NO.8 求每个学生（号）的平均成绩，并将其超过75分【HAVING AVG(score) > 75】的按学号输出【ORDER BY s_no】。 
*/
select s_no, AVG(score) as avg_score
from scores1
group by s_no
having AVG(score) > 75
order by s_no;

/*
NO.9 查询选修了课程1或者选修了课程2的学生姓名
*/
select name
from students1
where no in (
  select distinct s_no
  from scores1
  where c_no = '1' or c_no = '2');

/*
NO.10 查询既选修了课程1又选修了课程2的学生姓名【mysql模拟intersect: 用 DISTINCT,INNER JOIN  或 DISTINCT,WHERE等方式，可以实现交集操作即可】	
*/
select name 
from students1
where no in (
    select distinct s_no
    from scores1
    where c_no = '1' and s_no in (
    select distinct s_no
    from scores1
    where c_no = '2'
  )
);

/*
NO.11 查询选修Database这门课最高分学生所在的系名
*/
select dept.name
from students1 as stu inner join depts1 as dept on stu.d_no = dept.no
where stu.no in (
  select s_no
  from scores1
  where c_no = '1' and score in (
    select max(score)
    from scores1
    where c_no = '1'
  ) 
);


/*
NO.12 建立一个包含学生学号，姓名，年龄，以及所在系名的视图（赋予列名为sno,sname,sage,deptname）【create view】
*/
create view student_view as select s.no as sno, s.name as sname, s.age as sage, d.name as deptname from students1 s inner join depts1 d on s.d_no=d.no;
