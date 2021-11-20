create table t1(id int, age float);
create table t2(id int, age float);

insert into t1 values(1, 25.0);
insert into t1 values(2, 2.0);
insert into t1 values(2, 5.0);
insert into t1 values(2, 7.0);
insert into t2 values(1, 2.0);
insert into t2 values(1, 3.0);
select * from t1 where id not in (select t2.id from t2 where 1=0);
select * from t1 where t1.age > (select avg(t2.age) from t2) and t1.age <6.0;

SELECT * FROM t1 WHERE age < (SELECT MAX(t2.age) FROM t2 WHERE 1=0);
select * from t1 where id in (select t2.id from t2 where 1=0);
select * from t1 where id not in (select t2.id from t2 where 1=0);
select * from t1 where age < (select max(t2.age) from t2);
select * from t1 where id in (select t2.id from t2);
select * from t1 where id not in (select t2.id from t2);


SELECT * FROM t1 WHERE age = (SELECT t2.age FROM t2);
SELECT * FROM t1 WHERE age = (SELECT * FROM t2);
SELECT * FROM t1 WHERE age IN (SELECT * FROM t2);
SELECT * FROM t1 WHERE age NOT IN (SELECT * FROM t2);
