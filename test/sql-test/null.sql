create table t (a int, b int not null, c int nullable);
create table a (f int nullable, g int nullable);
create table b (f int nullable, g int nullable);
insert into t values (null, 1, 1);
insert into t values (1, null, 1);
insert into t values (1, 1, null);
insert into t values (1, 1, 1);
select * from t where 1 is null;