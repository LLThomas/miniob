create table t2(id int,name char,price int);
insert into t2 values (2, 'A',100);
insert into t2 values (2, 'B',100);
insert into t2 values (102, 'C',100);
update t2 set name ='D' where id = 2 and price = 100;
update t2 set name='E' where id > 100;
select * from t2;