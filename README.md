# MiniOB

[![MiniOB Pipeline](https://github.com/LLThomas/miniob/actions/workflows/cmake.yml/badge.svg)](https://github.com/LLThomas/miniob/actions/workflows/cmake.yml)

## Introduction

miniob设计的目标是让不熟悉数据库设计和实现的同学能够快速的了解与深入学习数据库内核，期望通过miniob相关训练之后，能够对各个数据库内核模块的功能与它们之间的关联有所了解，并能够在
使用时，设计出高效的SQL。面向的对象主要是在校学生，并且诸多模块做了简化，比如不考虑并发操作。 注意：此代码仅供学习使用，不考虑任何安全特性。

## How to build

please refer to docs/how_to_build.md

## 测试常见问题

[Link](https://github.com/OceanBase-Partner/lectures-on-dbms-implementation/blob/main/miniob-topics.md#%E6%B5%8B%E8%AF%95%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98)


## Assignments

### 必做题

| 状态 | 名称 | 描述 | 测试用例示例 |
| ------ | ---- | ---- | -------------|
| <ul><li>[ ] 以后再做 </li></ul> | 优化buffer pool | 必做。实现LRU淘汰算法或其它淘汰算法 | |
| <ul><li>[x] 完成 </li></ul> | 查询元数据校验 | 必做。查询语句中存在不存在的列名、表名等，需要返回失败。需要检查代码，判断是否需要返回错误的地方都返回错误了。 |create table t(id int, age int);<br/>select * from t where name='a'; <br/>select address from t where id=1;<br/>select * from t_1000;|
| <ul><li>[x] 完成 </li></ul> | drop table | 必做。删除表。清除表相关的资源。 |create table t(id int, age int);<br/>create table t(id int, name char);<br/>drop table t;<br/>create table t(id int, name char);|
| <ul><li>[x] 完成 </li></ul> | 实现update功能 | 必做。update单个字段即可。 |update t set age =100 where id=2;<br/>update set age=20 where id>100;|
| <ul><li>[x] 完成 </li></ul> | 增加date字段 | 必做。date测试不会超过2038年2月。注意处理非法的date输入，需要返回FAILURE。 |create table t(id int, birthday date);<br/>insert into t values(1, '2020-09-10');<br/>insert into t values(2, '2021-1-2');<br/>select * from t;|
| <ul><li>[x] 完成 </li></ul> | 多表查询 | 必做。支持多张表的笛卡尔积关联查询。需要实现select * from t1,t2; select t1.*,t2.* from t1,t2;以及select t1.id,t2.id from t1,t2;查询可能会带条件。查询结果展示格式参考单表查询。每一列必须带有表信息，比如:<br/>t1.id \|  t2.id <br/>1 \| 1 |select * from t1,t2;<br/>select * from t1,t2 where t1.id=t2.id and t1.age > 10;<br/>select * from t1,t2,t3;|
| <ul><li>[x] 完成 </li></ul> | 聚合运算 | 需要实现max/min/count/avg.<br/>包含聚合字段时，只会出现聚合字段。聚合函数中的参数不会是表达式，比如age +1 |select max(age) from t1; select count(*) from t1; select count(1) from t1; select count(id) from t1;|



### 选做题

| 状态 | 名称               | 分值 | 描述                                                         | 测试用例示例                                                 |
| ------ | --- | ---- | ------------- | ------------- |
| <ul><li>[x] 完成 </li></ul> | 多表join操作       | 20   | INNER JOIN。需要支持join多张表。需要考虑大表问题，不要直接使用笛卡尔积再过滤 | select * from t1 inner join t2 on t1.id=t2.id;<br/>select * from t1 inner join t2 on t1.id=t2.id inner join t3 on t1.id=t3.id;<br/>selec * from t1 inner join t2 on t1.id=t2.id and t2.age>10 where t1.name >='a'; |
| <ul><li>[x] 完成 </li></ul> | 一次插入多条数据   | 10   | 一次插入的数据要同时成功或失败。                             | insert into t1 values(1,1),(2,2),(3,3);                      |
| <ul><li>[x] 完成 </li></ul> | 唯一索引           | 10   | 唯一索引：create unique index                                | create unique index i_id on t1(id);<br/>insert into t1 values(1,1);<br/>insert into t1 values(1,2); -- failed |
| <ul><li>[x] 完成 </li></ul> | 支持NULL类型       | 10   | 包括但不限于建表、查询和插入。默认情况不允许为NULL，使用nullable关键字表示字段允许为NULL。<br/>Null不区分大小写 | create table t1 (id int not null, age int not null, address nullable); create table t1 (id int, age int, address char nullable); insert into t1 values(1,1, null); |
| <ul><li>[ ] 未认领 </li></ul> | 简单子查询         | 10   | 支持简单的IN(NOT IN)语句；<br/>支持与子查询结果做比较运算；<br/>支持子查询中带聚合函数。<br/>子查询中不会与主查询做关联。 | select * from t1 where name in(select name from t2);<br/>select * from t1 where t1.age >(select max(t2.age) from t2);<br/>select * from t1 where t1.age > (select avg(t2.age) from t2) and t1.age > 20.0; <br/>NOTE: 表达式中可能存在不同类型值比较 |
| <ul><li>[x] 完成 </li></ul> | 多列索引           | 20   | 单个索引关联了多个字段                                       | create index i_id on t1(id, age);                            |
| <ul><li>[x] 完成 </li></ul> | 超长字段           | 20   | 超长字段的长度可能超出一页，比如常见的text,blob等。这里仅要求实现text（text 长度固定4096字节），可以当做字符串实现。<br/>注意：当前的查询，只能支持一次返回少量数据，需要扩展 | create table t(id int, age int, info text);<br/>insert into t(1,1, 'a very very long string');<br/>select * from t where id=1; |
| <ul><li>[ ] 未认领 </li></ul> | 查询条件支持表达式 | 20   | 查询条件中支持运算表达式，这里的运算表达式包括 +-*/。<br/>仅支持基本数据的运算即可，不对date字段做考察。<br/>运算出现异常，按照NULL规则处理。<br/>只需要考虑select。 | select * from t1,t2 where t1.age +10 > t2.age *2 + 3-(t1.age +10)/3;<br/>select t1.col1+t2.col2 from t1,t2 where t1.age +10 > t2.age *2 + 3-(t1.age +10)/3; |
| <ul><li>[ ] 未认领 </li></ul> | 复杂子查询         | 20   | 子查询在WHERE条件中，子查询语句支持多张表与AND条件表达式，查询条件支持max/min等 | select * from t1 where age in (select id from t2 where t2.name in (select name from t3)) |
| <ul><li>[ ] 未认领 </li></ul> | 排序               | 10   | 支持oder by功能。不指定排序顺序默认为升序(asc)。<br/>不需要支持oder by字段为数字的情况，比如select * from t order by 1; | select * from t,t1 where t.id=t1.id order by t.id asc,t1.score desc; |
| <ul><li>[ ] 未认领 </li></ul> | 分组               | 20   | 支持group by功能。group by中的聚合函数也不要求支持表达式     | select t.id, t.name, avg(t.score),avg(t2.age) from t,t2 where t.id=t2.id group by t.id; |
