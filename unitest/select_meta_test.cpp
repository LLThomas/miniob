//
// Created by huaouo on 11/9/21.
//

#include <include/sql_test.h>

class SelectMetaTest : public SQLTest {};

TEST_F(SelectMetaTest, invalid_table) {
  ASSERT_EQ(ExecuteSql("select * from t;"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("select t.a from t;"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("create table t (a int);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("select tt.a from tt;"), "FAILURE\n");
}

TEST_F(SelectMetaTest, invalid_column) {
  ASSERT_EQ(ExecuteSql("create table t (a int);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("select b from t;"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("select t.b from t;"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("select a from t where t.c = t.a;"), "FAILURE\n");
}