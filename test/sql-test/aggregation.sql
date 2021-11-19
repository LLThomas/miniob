create table t(id int,  price float,name char,age date);
insert into t values (1,1.0,'A','1998-01-01');
insert into t values (1,2.0,'A','1998-01-01');
insert into t values (1,3.0,'B','1998-01-01');
insert into t values (2,2.0,'A','1999-01-01');
insert into t values (3,3.0,'B','2000-01-01');
insert into t values (4,4.0,'B','2001-01-01');
insert into t values (5,5.0,'B','2001-01-01');
create table t2(id int,  price float,name char,age date);
insert into t2 values (1,2.0,'A','1998-01-01');
insert into t2 values (1,3.0,'A','1998-01-01');
insert into t2 values (1,4.0,'B','1998-01-01');
insert into t2 values (2,5.0,'A','1999-01-01');
insert into t2 values (2,6.0,'C','1999-01-01');
insert into t2 values (2,7.0,'A','1999-01-01');
insert into t2 values (3,8.0,'B','2000-01-01');
insert into t2 values (3,9.0,'B','2000-01-01');
insert into t2 values (3,13.0,'B','2000-01-01');
insert into t2 values (4,14.0,'B','2001-01-01');
insert into t2 values (5,15.0,'B','2001-01-01');
SELECT t.id, t.name, AVG(t.price), AVG(t2.price) FROM t, t2 WHERE t.id=t2.id GROUP BY t.id, t.name;
select name,count(*) from t group by name,age;
select count(*) from t group by name;
select count(*),min(price) from t group by name;

select name,count(*) from t group by t.name,t.age;
select count(*) from t group by t.name;
select count(*),min(price) from t group by t.name;

-- select * from t order by t.name, t.age;
-- select * from t order by t.name desc, t.age asc;
select count(*) from t;
select count(id) from t;
select min(id) from t;
select max(id) from t;
select avg(id) from t;

select count(price) from t;
select min(price) from t;
select max(price) from t;
select avg(price) from t;

select avg(price), max(price), min(price) from t;
select avg(price), max(id), min(price) from t;
select avg(price), avg(id) from t;

select max(*) from t;
select min(*) from t;
select avg(*) from t;


drop table t;
create table t(id int , age int nullable);
insert into t values (1,null);
insert into t values (12,null);
insert into t values (13,null);
insert into t values (1,null);
insert into t values (1,3);
select count(age) from t;
select min(age) from t;
select max(age) from t;
select AVG(age) from t;
--select count(1) from t;
--select count(1.1) from t;
--select count('1998-01-01') from t;
--select * from t;
-- select min(name) from t;
-- select min(age) from t;

-- select max(name) from t;
-- select max(age) from t;

--select count(*,id) from t;
--select count() from t;
--select count(not_exists_col) from t;