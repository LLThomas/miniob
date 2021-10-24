//
// Created by huaouo on 10/23/21.
//

#include <gtest/gtest.h>
#include <include/sql_test.h>

// 继承一个新类
class DemoSQLTest : public SQLTest {};

TEST_F(DemoSQLTest, create_table) {
  ExecuteSql("create table t (a int);");
  ASSERT_EQ(ExecuteSql("show tables;"), "t\n\n");
}

TEST_F(DemoSQLTest, insert_data) {
  ExecuteSql("insert into t values (1);");
  ASSERT_EQ(ExecuteSql("select * from t;"), "a\n1\n");
}

TEST_F(DemoSQLTest, drop_table) {
  ExecuteSql("drop table t;");
  ASSERT_EQ(ExecuteSql("show tables;"), "No table\n");
}
