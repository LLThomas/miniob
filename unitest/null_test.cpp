//
// Created by huaouo on 11/8/21.
//
#include <include/sql_test.h>

class NullTest : public SQLTest {
  void BeforeCase() override {
    ExecuteSql("create table t (a int, b int not null, c int nullable);");
    ExecuteSql("create table a (f int nullable, g int nullable);");
    ExecuteSql("create table b (f int nullable, g int nullable);");
  }

  void AfterCase() override {
    ExecuteSql("drop table t;");
    ExecuteSql("drop table a;");
    ExecuteSql("drop table b;");
  }
};

TEST_F(NullTest, single_table) {
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
  ASSERT_EQ(ExecuteSql("select * from t where 1 is null;"), "a | b | c\n");
  ASSERT_EQ(ExecuteSql("select * from t where 1 is not null;"),
            "a | b | c\n1 | 1 | null\n1 | 1 | 1\n");
  ASSERT_EQ(ExecuteSql("select * from t where 1 = null;"), "a | b | c\n");
  ASSERT_EQ(ExecuteSql("select * from t where null = 1;"), "a | b | c\n");
}

TEST_F(NullTest, multi_table) {
  ASSERT_EQ(ExecuteSql("insert into a values (null, null);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into a values (1, 2);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into b values (null, null);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into b values (1, 3);"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("select a.g from a, b where a.f=b.f;"), "a.g\n2\n");
}