create table t(id int, age int);
create table tt(id int, age int);

insert into t values(1, 2);
insert into t values(2, 3);

insert into tt values(2, 2);
insert into tt values(3, 3);

select t.id, tt.age from t, tt;

select t.id, tt.age from t, tt order by t.id asc, tt.age desc;