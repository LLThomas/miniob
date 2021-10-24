create table t2(id int,age int);
insert into t2 values (2, 80);
insert into t2 values (102, 80);
update t2 set age =100 where id = 2;
update t2 set age=20 where id > 100;
select * from t2;