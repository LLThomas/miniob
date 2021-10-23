drop table pr1;
drop table pr2;
drop table pr3;
create table pr1(id int, age int);
create table pr2(id int, age int);
create table pr3(id int, age int);
insert into pr1 values (101, 70);
insert into pr1 values (102, 80);
insert into pr2 values (102, 70);
insert into pr2 values (102, 50);
insert into pr2 values (103, 70);
insert into pr3 values (103, 10);
insert into pr3 values (104, 20);
select * from pr1,pr2;
select * from pr1,pr2 where pr1.id=pr2.id;
select * from pr1,pr2 where pr1.id=pr2.id and pr2.age > 60;
select pr1.id,pr2.age from pr1,pr2;
select * from pr1,pr2,pr3;