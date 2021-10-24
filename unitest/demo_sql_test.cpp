//
// Created by huaouo on 10/23/21.
//

#include <gtest/gtest.h>
#include <include/sql_test.h>

// 继承一个新类
class DemoSQLTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void SetUp() override { ExecuteSql("create table t (a int);"); }

  // 在每个 TEST_F 之后执行的内容
  void TearDown() override { ExecuteSql("drop table t;"); }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(DemoSQLTest, insert_1) {
  ExecuteSql("insert into t values (1);");
  ASSERT_EQ(ExecuteSql("select * from t;"), "a\n1\n");
}

TEST_F(DemoSQLTest, insert_10086) {
  ExecuteSql("insert into t values (10086);");
  ASSERT_EQ(ExecuteSql("select * from t;"), "a\n10086\n");
}
