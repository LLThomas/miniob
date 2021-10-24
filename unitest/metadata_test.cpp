//
// Created by thomas on 2021/10/24.
//

#include <include/sql_test.h>

class MetaDataTest : public SQLTest {
  void SetUp() {
    ExecuteSql("create table t (id int, age int);");
  }

  void TearDown() {
    ExecuteSql("drop table t;");
  }
};

TEST_F(MetaDataTest, select_empty) {
  ExecuteSql("insert into t values(1,1);");
  ExecuteSql("delete from t where id=1;");
  ASSERT_EQ(ExecuteSql("select * from t;"), "id | age\n");
}