//
// Created by huaouo on 11/8/21.
//
#include <include/sql_test.h>

class NullTest : public SQLTest {
  void BeforeCase() override {
    ExecuteSql("create table t (a int, b int not null, c int nullable);");
  }

  void AfterCase() override { ExecuteSql("drop table t;"); }
};

TEST_F(NullTest, no_index) {
  ASSERT_EQ(ExecuteSql("insert into t values (null, 1, 1);"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1, null, 1);"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1, 1, null);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1, 1, 1);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("select * from t;"),
            "a | b | c\n1 | 1 | null\n1 | 1 | 1\n");
  ASSERT_EQ(ExecuteSql("select * from t where c is null;"),
            "a | b | c\n1 | 1 | null\n");
  ASSERT_EQ(ExecuteSql("select * from t where c is not null;"),
            "a | b | c\n1 | 1 | 1\n");
}
