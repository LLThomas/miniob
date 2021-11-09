create table t(id int,age float);
insert into t values(1,1.111);
insert into t values(2,2.222);
insert into t values(3,3.333);
insert into t values(4,4.444);
insert into t values(5,5.555);
insert into t values(6,6.666);

select id,age from t;
select t.id,age from t;
select * from t;
select * from t where id<3;
select age from t where id>3;
select * from t where id>3 and age<6.0;
