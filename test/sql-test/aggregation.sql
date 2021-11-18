create table t(id int, name char, price float,age date);
insert into t values (1,'ali',1.111,'1998-01-01');
insert into t values (2,'bob',2.222,'1999-01-01');
insert into t values (3,'cin',3.333,'2000-01-01');
insert into t values (4,'def',4.444,'2001-01-01');

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