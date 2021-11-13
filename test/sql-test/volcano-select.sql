create table t(id int,age float);
insert into t values(1,1.111);
insert into t values(2,2.222);
insert into t values(3,3.333);
insert into t values(4,4.444);
insert into t values(5,5.555);
insert into t values(6,6.666);

create table t1(id int,age float);
insert into t1 values(3,3.555);
insert into t1 values(4,4.666);
insert into t1 values(5,5.777);
insert into t1 values(6,6.888);
insert into t1 values(7,7.123);
insert into t1 values(8,8.234);

select id,age from t;
select t.id,age from t;
select * from t;
select * from t where id<3;
select age from t where id>3;
select * from t where id>3 and age<6.0;

select t.icy from t;
select icy from t;
select * from icy;
select * from t where icy.id<3;
select * from t where t.icy<3;
select * from t where icy<3; 

select t1.age from t inner join t1 on t.id=t1.id;