//
// Created by huaouo on 10/27/21.
//

#include <include/sql_test.h>

// 继承一个新类
class DateTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void BeforeCase() override {
    ExecuteSql("create table t (a date);");
    ExecuteSql("insert into t values ('2000-01-01');");
    ExecuteSql("insert into t values ('2000-01-02');");
    ExecuteSql("insert into t values ('2000-01-03');");
    ExecuteSql("insert into t values ('2000-01-04');");
  }

  // 在每个 TEST_F 之后执行的内容
  void AfterCase() override { ExecuteSql("drop table t;"); }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(DateTest, date_compare) {
  ASSERT_EQ(ExecuteSql("select * from t where a > '2000-01-03';"),
            "a\n2000-01-04\n");
  ASSERT_EQ(ExecuteSql("select * from t where '2000-01-03' = '2000-01-03';"),
            "a\n2000-01-01\n2000-01-02\n2000-01-03\n2000-01-04\n");
  ASSERT_EQ(ExecuteSql("select * from t where '2000-01-03' < a;"),
            "a\n2000-01-04\n");
  ASSERT_EQ(ExecuteSql("select * from t where a >= '2000-01-03';"),
            "a\n2000-01-03\n2000-01-04\n");
  ASSERT_EQ(ExecuteSql("select * from t where a = '2000-01-03';"),
            "a\n2000-01-03\n");
  ASSERT_EQ(ExecuteSql("select * from t where a <= '2000-01-02';"),
            "a\n2000-01-01\n2000-01-02\n");
  ASSERT_EQ(ExecuteSql("select * from t where a < '2000-01-02';"),
            "a\n2000-01-01\n");
  ASSERT_EQ(ExecuteSql("select * from t where a < '1999-10-31';"), "a\n");
  ASSERT_EQ(ExecuteSql("select * from t where a <> '2000-01-03';"),
            "a\n2000-01-01\n2000-01-02\n2000-01-04\n");
  ASSERT_EQ(
      ExecuteSql(
          "select * from t where a < '2000-01-03' and a >= '2000-01-02';"),
      "a\n2000-01-02\n");
  ASSERT_EQ(
      ExecuteSql(
          "select * from t where a < '2000-01-03' and a >= '2000-01-03';"),
      "a\n");
  //  ASSERT_EQ(ExecuteSql("select * from t where a < '2000-01-02' or a >
  //  '2000-01-03';"), "a\n2000-01-02\n");
}
