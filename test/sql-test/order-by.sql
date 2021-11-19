create table t(id int, age int);
create table tt(id int, age int);

insert into t values(2, 2);
insert into t values(1, 3);

insert into tt values(2, 2);
insert into tt values(3, 3);


select * from t, tt order by t.id, tt.id;
