create table A(id int,age float);
insert into A values(1,1.111);
insert into A values(2,2.222);
insert into A values(3,3.333);
insert into A values(4,4.444);
insert into A values(5,5.555);
insert into A values(6,6.666);

create table B(id int,age float);
insert into B values(3,3.555);
insert into B values(4,4.666);
insert into B values(5,5.777);
insert into B values(6,6.888);
insert into B values(7,7.123);
insert into B values(8,8.234);

select B.id from A inner join B on A.id=B.id;
select A.id from A inner join B on A.id=B.id;
select A.age from A inner join B on A.id=B.id;
select B.age from A inner join B on A.id=B.id;

select id,age from A;
select A.id,age from A;
select * from A;
select * from A where id<3;
select age from A where id>3;
select * from A where id>3 and age<6.0;

select A.icy from A;
select icy from A;
select * from icy;
select * from A where icy.id<3;
select * from A where A.icy<3;
select * from A where icy<3; 

select B.id from A inner join B on A.id=B.id;