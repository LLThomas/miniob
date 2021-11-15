create table A(aid int,age float);
insert into A values(1,1.111);
insert into A values(2,2.222);
insert into A values(3,3.333);
insert into A values(4,4.444);
insert into A values(5,5.555);
insert into A values(6,6.666);

create table B(bid int,age float);
insert into B values(3,3.555);
insert into B values(4,4.666);
insert into B values(5,5.777);
insert into B values(6,6.888);
insert into B values(7,7.123);
insert into B values(8,8.234);

select * from A where aid>3 and age<6.0;
select * from A where aid<3;
select aid,age from A;
select * from A;
select age from A where aid>3;

select A.age from A inner join B on A.aid=B.bid;
select A.* from A inner join B on A.aid=B.bid;
select A.aid from A inner join B on A.aid=B.bid;
select B.bid from A inner join B on A.aid=B.bid;
select B.age from A inner join B on A.aid=B.bid;
select B.* from A inner join B on A.aid=B.bid;

select A.icy from A;
select icy from A;
select * from icy;
select * from A where icy.id<3;
select * from A where A.icy<3;
select * from A where icy<3; 
