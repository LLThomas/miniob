//
// Created by huaouo on 10/23/21.
//

#include <include/sql_test.h>

// 继承一个新类
class AggregationSQLTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void BeforeCase() override {
    ExecuteSql("create table t (a int);");
    ExecuteSql("insert into t values(1);");
    ExecuteSql("insert into t values(2);");
    ExecuteSql("insert into t values(3);");
  }

  // 在每个 TEST_F 之后执行的内容
  void AfterCase() override { ExecuteSql("drop table t;"); }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(AggregationSQLTest, max) {
  ASSERT_EQ(ExecuteSql("select max(*) from t;"), "a\n1\n");
}
