create table t1(id int, age int);
create table t2(id int, age int);

insert into t1 values(1, 25);
insert into t1 values(2, 7);
insert into t2 values(1, 2);
insert into t2 values(1, 3);

select * from t1 where t1.age in (select avg(t2.age) from t2) and t1.age > 20.0;
