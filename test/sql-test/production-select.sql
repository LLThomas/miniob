create table pr1(id int, price float);
create table pr2(id int, price float);
create table pr3(id int, price float);
insert into pr1 values (1, 7.000);
insert into pr1 values (2, 8.000);
insert into pr2 values (1, 5.000);
insert into pr2 values (1, 2.000);
insert into pr2 values (2, 7.000);
insert into pr2 values (2, 1.000);
insert into pr2 values (3, 7.567);
insert into pr3 values (3, 1.678);
insert into pr3 values (4, 2.789);

select * from pr1,pr2 where pr1.id=pr2.id;
select pr1.id ,avg(pr1.price),avg(pr2.price) from pr1,pr2 where pr1.id=pr2.id group by pr1.id;
select * from pr1,pr2 where pr1.id>=pr2.id;
select * from pr1,pr2;
select * from pr1,pr2 where pr1.id=pr2.id and pr2.price > 6.0;
select pr1.id,pr2.price from pr1,pr2;
select pr1.id,pr2.price,pr1.price from pr1,pr2;
select pr2.*,pr1.price from pr1,pr2;
select * from pr1,pr2,pr3;
select pr2.id,pr3.*,pr1.price from pr1,pr2,pr3;
select pr2.id from pr1,pr2 where pr1.id<3 and pr2.price=7.345 and pr1.id=pr2.id;