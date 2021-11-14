//
// Created by huaouo on 11/14/21.
//

#include <include/sql_test.h>

class TextTest : public SQLTest {
  void BeforeCase() override {
    ASSERT_EQ(ExecuteSql("create table t (a text);"), "SUCCESS\n");
  }

  void AfterCase() override {
    ASSERT_EQ(ExecuteSql("drop table t;"), "SUCCESS\n");
  }
};

TEST_F(TextTest, text) {
  ASSERT_EQ(ExecuteSql("insert into t values ('111');"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into t values ('222');"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("select * from t;"), "a\n111\n222\n");
  ASSERT_EQ(ExecuteSql("select * from t where a = '222';"), "a\n222\n");
  ASSERT_EQ(ExecuteSql("update t set a = '333' where a = '222';"), "SUCCESS\n");
  ASSERT_EQ(ExecuteSql("select * from t;"), "a\n111\n333\n");
}