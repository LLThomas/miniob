//
// Created by huaouo on 10/23/21.
//  \\n"\n"

#include <include/sql_test.h>

// 继承一个新类
class HashJoinTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void BeforeCase() override {
    ExecuteSql("create table A(aid int,age float);");
    ExecuteSql("insert into A values(1,1.111);");
    ExecuteSql("insert into A values(2,2.222);");
    ExecuteSql("insert into A values(3,3.333);");
    ExecuteSql("insert into A values(4,4.444);");
    ExecuteSql("insert into A values(5,5.555);");
    ExecuteSql("insert into A values(6,6.666);");
    ExecuteSql("create table B(bid int,age float);");
    ExecuteSql("insert into B values(3,3.555);");
    ExecuteSql("insert into B values(4,4.666);");
    ExecuteSql("insert into B values(5,5.777);");
    ExecuteSql("insert into B values(6,6.888);");
    ExecuteSql("insert into B values(7,7.123);");
    ExecuteSql("insert into B values(8,8.234);");
  }

  // 在每个 TEST_F 之后执行的内容
  void AfterCase() override {
    ExecuteSql("drop table A;");
    ExecuteSql("drop table B;");
  }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(HashJoinTest, hash_join) {
  ASSERT_EQ(ExecuteSql("select A.age from A inner join B on A.aid=B.bid;"),
            "A.age\n"
            "3.33\n"
            "4.44\n"
            "5.55\n"
            "6.67\n");
  ASSERT_EQ(ExecuteSql("select A.* from A inner join B on A.aid=B.bid;"),
            "A.aid | A.age\n"
            "3 | 3.33\n"
            "4 | 4.44\n"
            "5 | 5.55\n"
            "6 | 6.67\n");
  ASSERT_EQ(ExecuteSql("select A.aid from A inner join B on A.aid=B.bid;"),
            "A.aid\n"
            "3\n"
            "4\n"
            "5\n"
            "6\n");
  ASSERT_EQ(ExecuteSql("select B.bid from A inner join B on A.aid=B.bid;"),
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
