create table t(id int, age int);

insert into t values(6, 3);
insert into t values(1, 2);
insert into t values(1, 5);
insert into t values(1, 3);

select * from t order by id, age;