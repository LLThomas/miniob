//
// Created by huaouo on 10/23/21.
//

#include <include/sql_test.h>

// 继承一个新类
class ProductionSQLTest : public SQLTest {
  // 在每个 TEST_F 之前执行的内容
  void BeforeCase() override {
    ExecuteSql("create table pr1(id int, price float);");
    ExecuteSql("create table pr2(id int, price float);");
    ExecuteSql("create table pr3(id int, price float);");
    ExecuteSql("insert into pr1 values (1, 7.123);");
    ExecuteSql("insert into pr1 values (2, 8.234);");
    ExecuteSql("insert into pr2 values (2, 7.345);");
    ExecuteSql("insert into pr2 values (2, 5.456);");
    ExecuteSql("insert into pr2 values (3, 7.567);");
    ExecuteSql("insert into pr3 values (3, 1.678);");
    ExecuteSql("insert into pr3 values (4, 2.789);");
  }

  // 在每个 TEST_F 之后执行的内容
  void AfterCase() override {
    ExecuteSql("drop table pr1;");
    ExecuteSql("drop table pr2;");
    ExecuteSql("drop table pr3;");
  }
};

// 不对 TEST_F 的执行顺序提供保证
TEST_F(ProductionSQLTest, production) {
  ASSERT_EQ(ExecuteSql("select * from pr1,pr2;"),
            "pr1.id | pr1.price | pr2.id | pr2.price\n"
            "1 | 7.12 | 2 | 7.34\n"
            "1 | 7.12 | 2 | 5.46\n"
            "1 | 7.12 | 3 | 7.57\n"
            "2 | 8.23 | 2 | 7.34\n"
            "2 | 8.23 | 2 | 5.46\n"
            "2 | 8.23 | 3 | 7.57\n");
  ASSERT_EQ(ExecuteSql("select pr1.id from pr1,pr2;"),
            "pr1.id\n"
            "1\n"
            "1\n"
            "1\n"
            "2\n"
            "2\n"
            "2\n");
  ASSERT_EQ(ExecuteSql("select * from pr1,pr2 where pr1.id=pr2.id;"),
            "pr1.id | pr1.price | pr2.id | pr2.price\n"
            "2 | 8.23 | 2 | 7.34\n"
            "2 | 8.23 | 2 | 5.46\n");
  ASSERT_EQ(
      ExecuteSql(
          "select * from pr1,pr2 where pr1.id=pr2.id and pr2.price > 6.0;"),
      "pr1.id | pr1.price | pr2.id | pr2.price\n"
      "2 | 8.23 | 2 | 7.34\n");
  ASSERT_EQ(ExecuteSql("select pr1.id,pr2.price from pr1,pr2;"),
            "pr1.id | pr2.price\n"
            "1 | 7.34\n"
            "1 | 5.46\n"
            "1 | 7.57\n"
            "2 | 7.34\n"
            "2 | 5.46\n"
            "2 | 7.57\n");
  ASSERT_EQ(ExecuteSql("select pr1.id,pr2.price,pr1.price from pr1,pr2;"),
            "pr1.id | pr2.price | pr1.price\n"
            "1 | 7.34 | 7.12\n"
            "1 | 5.46 | 7.12\n"
            "1 | 7.57 | 7.12\n"
            "2 | 7.34 | 8.23\n"
            "2 | 5.46 | 8.23\n"
            "2 | 7.57 | 8.23\n");
  ASSERT_EQ(ExecuteSql("select pr2.*,pr1.price from pr1,pr2;"),
            "pr2.id | pr2.price | pr1.price\n"
            "2 | 7.34 | 7.12\n"
            "2 | 5.46 | 7.12\n"
            "3 | 7.57 | 7.12\n"
            "2 | 7.34 | 8.23\n"
            "2 | 5.46 | 8.23\n"
            "3 | 7.57 | 8.23\n");
  ASSERT_EQ(ExecuteSql("select * from pr1,pr2,pr3;"),
            "pr1.id | pr1.price | pr2.id | pr2.price | pr3.id | pr3.price\n"
            "1 | 7.12 | 2 | 7.34 | 3 | 1.68\n"
            "1 | 7.12 | 2 | 7.34 | 4 | 2.79\n"
            "1 | 7.12 | 2 | 5.46 | 3 | 1.68\n"
            "1 | 7.12 | 2 | 5.46 | 4 | 2.79\n"
            "1 | 7.12 | 3 | 7.57 | 3 | 1.68\n"
            "1 | 7.12 | 3 | 7.57 | 4 | 2.79\n"
            "2 | 8.23 | 2 | 7.34 | 3 | 1.68\n"
            "2 | 8.23 | 2 | 7.34 | 4 | 2.79\n"
            "2 | 8.23 | 2 | 5.46 | 3 | 1.68\n"
            "2 | 8.23 | 2 | 5.46 | 4 | 2.79\n"
            "2 | 8.23 | 3 | 7.57 | 3 | 1.68\n"
            "2 | 8.23 | 3 | 7.57 | 4 | 2.79\n");
  ASSERT_EQ(ExecuteSql("select pr2.id,pr3.*,pr1.price from pr1,pr2,pr3;"),
            "pr2.id | pr3.id | pr3.price | pr1.price\n"
            "2 | 3 | 1.68 | 7.12\n"
            "2 | 4 | 2.79 | 7.12\n"
            "2 | 3 | 1.68 | 7.12\n"
            "2 | 4 | 2.79 | 7.12\n"
            "3 | 3 | 1.68 | 7.12\n"
            "3 | 4 | 2.79 | 7.12\n"
            "2 | 3 | 1.68 | 8.23\n"
            "2 | 4 | 2.79 | 8.23\n"
            "2 | 3 | 1.68 | 8.23\n"
            "2 | 4 | 2.79 | 8.23\n"
            "3 | 3 | 1.68 | 8.23\n"
            "3 | 4 | 2.79 | 8.23\n");
}
