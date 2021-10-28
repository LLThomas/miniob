//
// Created by huaouo on 10/23/21.
//

#include <include/sql_test.h>

// 继承一个新类
class AggregationSQLTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void BeforeCase() override {
    ExecuteSql("create table t(id int, name char, price float,age date);");
    ExecuteSql("insert into t values (1,'ali',1.111,'1998-01-01');");
    ExecuteSql("insert into t values (2,'bob',2.222,'1999-01-01');");
    ExecuteSql("insert into t values (3,'cin',3.333,'2000-01-01');");
    ExecuteSql("insert into t values (4,'def',4.444,'2001-01-01');");
  }

  // 在每个 TEST_F 之后执行的内容
  void AfterCase() override { ExecuteSql("drop table t;"); }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(AggregationSQLTest, aggregation) {
  ASSERT_EQ(ExecuteSql("select * from t;"),
            "id | name | price | age\n"
            "1 | ali | 1.11 | 1998-01-01\n"
            "2 | bob | 2.22 | 1999-01-01\n"
            "3 | cin | 3.33 | 2000-01-01\n"
            "4 | def | 4.44 | 2001-01-01\n");
  ASSERT_EQ(ExecuteSql("select count(*) from t;"), "count(*)\n4\n");
  ASSERT_EQ(ExecuteSql("select count(1) from t;"), "count(1)\n4\n");
  ASSERT_EQ(ExecuteSql("select count(id) from t;"), "count(id)\n4\n");
  ASSERT_EQ(ExecuteSql("select min(id) from t;"), "min(id)\n1\n");
  ASSERT_EQ(ExecuteSql("select min(name) from t;"), "min(name)\nali\n");
  ASSERT_EQ(ExecuteSql("select min(age) from t;"), "min(age)\n1998-01-01\n");
  ASSERT_EQ(ExecuteSql("select max(id) from t;"), "max(id)\n4\n");
  ASSERT_EQ(ExecuteSql("select max(name) from t;"), "max(name)\ndef\n");
  ASSERT_EQ(ExecuteSql("select max(age) from t;"), "max(age)\n2001-01-01\n");
  ASSERT_EQ(ExecuteSql("select avg(id) from t;"), "avg(id)\n2.5\n");
  ASSERT_EQ(ExecuteSql("select avg(price) from t;"), "avg(price)\n2.78\n");
  ASSERT_EQ(ExecuteSql("select avg(price), max(price), min(price) from t;"),
            "avg(price) | max(price) | min(price)\n"
            "2.78 | 4.44 | 1.11\n");
  ASSERT_EQ(ExecuteSql("select avg(price), max(id), min(name) from t;"),
            "avg(price) | max(id) | min(name)\n2.78 | 4 | ali\n");
  ASSERT_EQ(ExecuteSql("select count(*,id) from t;"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("select count() from t;"), "FAILURE\n");
  ASSERT_EQ(ExecuteSql("select count(not_exists_col) from t;"), "FAILURE\n");
}
