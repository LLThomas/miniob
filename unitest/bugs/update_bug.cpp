//
// Created by huaouo on 10/28/21.
//

#include "include/sql_test.h"

class UpdateBug : public SQLTest {};

TEST_F(UpdateBug, duplicate_update) {
  ASSERT_EQ(ExecuteSql("create table t (a int);"),"SUCCESS\n");
  ASSERT_EQ(ExecuteSql("create index a_id on t(a);"),"SUCCESS\n");
  ASSERT_EQ(ExecuteSql("insert into t values(1);"),"SUCCESS\n");
  ASSERT_EQ(ExecuteSql("update t set a = 2 where a = 1;"),"SUCCESS\n");
  ASSERT_EQ(ExecuteSql("select * from t;"),"a\n2\n");
  ASSERT_EQ(ExecuteSql("select * from t where a > 0;"),"a\n2\n");
}
