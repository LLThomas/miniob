//
// Created by huaouo on 10/23/21.
//

#include <include/sql_test.h>

// 继承一个新类
class BasicTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void BeforeCase() override {
    ExecuteSql("create table A(aid int,age float);");
    ExecuteSql("insert into A values(1,1.111);");
    ExecuteSql("insert into A values(2,2.222);");
    ExecuteSql("insert into A values(3,3.333);");
    ExecuteSql("insert into A values(4,4.444);");
    ExecuteSql("insert into A values(5,5.555);");
    ExecuteSql("insert into A values(6,6.666);");
  }

  // 在每个 TEST_F 之后执行的内容
  void AfterCase() override { ExecuteSql("drop table A;"); }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(BasicTest, basic_sql) {
  ASSERT_EQ(ExecuteSql("select * from A where aid>3 and age<6.0;"),
            "aid | age\n"
            "4 | 4.44\n"
            "5 | 5.55\n");
  ASSERT_EQ(ExecuteSql("select * from A where aid<3;"),
            "aid | age\n"
            "1 | 1.11\n"
            "2 | 2.22\n");
  ASSERT_EQ(ExecuteSql("select aid,age from A;"),
            "aid | age\n"
            "1 | 1.11\n"
            "2 | 2.22\n"
            "3 | 3.33\n"
            "4 | 4.44\n"
            "5 | 5.55\n"
            "6 | 6.67\n");
  ASSERT_EQ(ExecuteSql("select * from A;"),
            "aid | age\n"
            "1 | 1.11\n"
            "2 | 2.22\n"
            "3 | 3.33\n"
            "4 | 4.44\n"
            "5 | 5.55\n"
            "6 | 6.67\n");
  ASSERT_EQ(ExecuteSql("select age from A where aid>3;"),
            "age\n"
            "4.44\n"
            "5.55\n"
            "6.67\n");
}
