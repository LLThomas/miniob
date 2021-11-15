create table SELECT_TABLES_1(ID int,AGE int, U_NAME char);
create table SELECT_TABLES_2(ID int,AGE int, U_NAME char);
create table SELECT_TABLES_3(ID int,RES int, U_NAME char);
insert into SELECT_TABLES_1 values (1, 18,'A');
insert into SELECT_TABLES_1 values (2, 15,'B');
insert into SELECT_TABLES_2 values (2, 21,'C');
insert into SELECT_TABLES_2 values (1, 20,'A');
insert into SELECT_TABLES_3 values (1, 35,'A');
insert into SELECT_TABLES_3 values (2, 37,'A');

SELECT SELECT_TABLES_1.ID,SELECT_TABLES_2.U_NAME,SELECT_TABLES_3.RES FROM SELECT_TABLES_1,SELECT_TABLES_2,SELECT_TABLES_3;