//
// Created by huaouo on 10/27/21.
//

#include <include/sql_test.h>

class MultiInsertTest : public SQLTest {
  void BeforeCase() override { ExecuteSql("create table t (a int, b int);"); }

  void AfterCase() override { ExecuteSql("drop table t;"); }
};

TEST_F(MultiInsertTest, insert_success) {
  ASSERT_EQ(ExecuteSql("insert into t values (1,1),(2,2);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("select a from t;"), "a\n1\n2\n");
}

TEST_F(MultiInsertTest, insert_failure) {
  ASSERT_EQ(ExecuteSql("insert into t values (1,1),('f',2);"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1,1),(2);"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1,1),();"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("select a from t;"), "a\n");
}
