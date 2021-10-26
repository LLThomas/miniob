//
// Created by huaouo on 10/23/21.
//

#include <include/sql_test.h>

// 继承一个新类
class AggregationSQLTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void BeforeCase() override {
    ExecuteSql("create table t(id int);");
    ExecuteSql("insert into t values(1);");
    ExecuteSql("insert into t values(2);");
    ExecuteSql("insert into t values(3);");
    ExecuteSql("insert into t values(4);");
  }

  // 在每个 TEST_F 之后执行的内容
  void AfterCase() override { ExecuteSql("drop table t;"); }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(AggregationSQLTest, aggregation) {
  ASSERT_EQ(ExecuteSql("select count(*) from t;"), "count(*)\n4\n");
  ASSERT_EQ(ExecuteSql("select count(1) from t;"), "count(1)\n4\n");
  ASSERT_EQ(ExecuteSql("select count(id) from t;"), "count(id)\n4\n");
  ASSERT_EQ(ExecuteSql("select min(id) from t;"), "min(id)\n1\n");
  ASSERT_EQ(ExecuteSql("select max(id) from t;"), "max(id)\n4\n");
  ASSERT_EQ(ExecuteSql("select avg(id) from t;"), "avg(id)\n2.5\n");
}
