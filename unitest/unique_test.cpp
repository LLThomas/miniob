//
// Created by huaouo on 11/13/21.
//

#include <include/sql_test.h>

class UniqueTest : public SQLTest {};

TEST_F(UniqueTest, fail_insert) {
  ASSERT_EQ(ExecuteSql("create table t (a int nullable);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("create unique index a_id on t(a);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1);"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("drop table t;"), "SUCCESS\n");
}

TEST_F(UniqueTest, fail_create_index) {
  ASSERT_EQ(ExecuteSql("create table t (a int nullable);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into t values (1);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("drop table t;"), "SUCCESS\n");
}