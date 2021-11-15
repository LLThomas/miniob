//
// Created by huaouo on 10/23/21.
//

#include <include/sql_test.h>

// 继承一个新类
class HashJoinTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void BeforeCase() override {
    ExecuteSql("create table A(id int,age float,name char);");
    ExecuteSql("insert into A values(1,1.111,'a');");
    ExecuteSql("insert into A values(2,2.222,'b');");
    ExecuteSql("insert into A values(3,3.333,'a');");
    ExecuteSql("insert into A values(4,4.444,'b');");
    ExecuteSql("insert into A values(3,3.333,'b');");
    ExecuteSql("insert into A values(4,4.444,'a');");
    ExecuteSql("create table B(id int,age float,name char);");
    ExecuteSql("insert into B values(3,3.555,'cc');");
    ExecuteSql("insert into B values(4,4.666,'dd');");
    ExecuteSql("insert into B values(4,5.777,'ee');");
    ExecuteSql("insert into B values(6,6.888,'ff');");
    ExecuteSql("create table C(id int,age float,name char);");
    ExecuteSql("insert into C values(5,5.567,'eee');");
    ExecuteSql("insert into C values(6,6.678,'fff');");
    ExecuteSql("insert into C values(7,7.789,'ggg');");
    ExecuteSql("insert into C values(8,8.201,'hhh');");
  }

  // 在每个 TEST_F 之后执行的内容
  void AfterCase() override {
    ExecuteSql("drop table A;");
    ExecuteSql("drop table B;");
    ExecuteSql("drop table C;");
  }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(HashJoinTest, hash_join) {
  ASSERT_EQ(ExecuteSql("SELECT * FROM A INNER JOIN B ON A.id=B.id;"),
            "A.id | A.age | A.name | B.id | B.age | B.name\n"
            "3 | 3.33 | a | 3 | 3.56 | cc\n"
            "3 | 3.33 | b | 3 | 3.56 | cc\n"
            "4 | 4.44 | b | 4 | 4.67 | dd\n"
            "4 | 4.44 | a | 4 | 4.67 | dd\n"
            "4 | 4.44 | b | 4 | 5.78 | ee\n"
            "4 | 4.44 | a | 4 | 5.78 | ee\n");
  ASSERT_EQ(ExecuteSql("SELECT A.name FROM A INNER JOIN B ON A.id=B.id;"),
            "A.name\n"
            "a\n"
            "b\n"
            "b\n"
            "a\n"
            "b\n"
            "a\n");
  ASSERT_EQ(ExecuteSql("SELECT B.age FROM A INNER JOIN B ON A.id=B.id;"),
            "B.age\n"
            "3.56\n"
            "3.56\n"
            "4.67\n"
            "4.67\n"
            "5.78\n"
            "5.78\n");
  ASSERT_EQ(ExecuteSql("SELECT * FROM A INNER JOIN B ON A.id=B.id INNER JOIN C "
                       "ON A.id=C.id;"),
            "B.bid\n"
            "3\n"
            "4\n"
            "5\n"
            "6\n");
  ASSERT_EQ(ExecuteSql("select B.age from A inner join B on A.aid=B.bid;"),
            "B.age\n"
            "3.56\n"
            "4.67\n"
            "5.78\n"
            "6.89\n");
  ASSERT_EQ(ExecuteSql("select B.* from A inner join B on A.aid=B.bid;"),
            "B.bid | B.age\n"
            "3 | 3.56\n"
            "4 | 4.67\n"
            "5 | 5.78\n"
            "6 | 6.89\n");
}
