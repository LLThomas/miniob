create table SELECT_TABLES_4(ID int,AGE int, U_NAME char);
create table SELECT_TABLES_5(ID int,AGE int, U_NAME char);
insert into SELECT_TABLES_4 values (1, 2,'A');
insert into SELECT_TABLES_4 values (1, 3,'B');
insert into SELECT_TABLES_4 values (2, 2,'C');
insert into SELECT_TABLES_4 values (2, 4,'D');
insert into SELECT_TABLES_5 values (1, 10,'G');
insert into SELECT_TABLES_5 values (1, 11,'F');
insert into SELECT_TABLES_5 values (2, 12,'C');

SELECT * FROM SELECT_TABLES_4;