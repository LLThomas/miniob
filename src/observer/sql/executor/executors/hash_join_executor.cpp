#include "sql/executor/executors/hash_join_executor.h"

#include <string>
#include <unordered_map>

#include "sql/executor/expressions/comparison_expression.h"

HashJoinExecutor::HashJoinExecutor(
    ExecutorContext *exec_ctx, HashJoinPlanNode *plan,
    std::unique_ptr<AbstractExecutor> &&left_child,
    std::unique_ptr<AbstractExecutor> &&right_child)
    : AbstractExecutor(exec_ctx),
      plan_(plan),
      left_(std::move(left_child)),
      right_(std::move(right_child)) {}

void HashJoinExecutor::Init() {
  left_->Init();
  // 1. build
  std::string left_hashkey = plan_->GetLeftColName();
  int col_index = left_hashkey == ""
                      ? -1
                      : left_->GetOutputSchema()->GetFieldIdx(left_hashkey);
  Tuple temp_tuple;
  RID rid;
  rid.page_num = 1;
  rid.slot_num = -1;
  while (left_->Next(&temp_tuple, &rid) == RC::SUCCESS) {
    std::string s;
    if (col_index != -1) {
      const TupleValue *tuple_value = &temp_tuple.get(col_index);
      if (dynamic_cast<const NullValue *>(tuple_value)) {
        continue;
      }
      std::stringstream ss;
      tuple_value->to_string(ss);
      ss >> s;
    }

    std::cout << "build: " << s << std::endl;

    hash_map[s].emplace_back(new Tuple(std::move(temp_tuple)));
  }
  LOG_ERROR("break\n");

  right_->Init();
}

RC HashJoinExecutor::Next(Tuple *tuple, RID *rid) {
  LOG_ERROR("right Next\n");

  // check last_tuples first
  Tuple *next_tuple;
  bool is_matched = false;
  while (!is_matched) {
    //这个while如果没有JoinExpression，只做一次，相当于笔直向下走和原来是一样的
    //但是如果出现不匹配的元组，就会一直想方设法获取下一个匹配的
    next_tuple = GetNextTuple();
    if (next_tuple == nullptr) break;
    is_matched = true;
    for (auto &p : plan_->JoinKeyExpression()) {
      if (p->EvaluateJoin(next_tuple, plan_->GetChildAt(0)->OutputSchema(),
                          &last_right_tuple,
                          plan_->GetChildAt(1)->OutputSchema())
              ->compare(IntValue(true)) != 0) {
        is_matched = false;
        break;
      }
    }
  }
  if (!is_matched) {
    //说明是nullptr跳出来的
  } else {
    //说明是匹配成功了
    //组合
    Tuple big_tuple;
    for (int i = 0; i < next_tuple->size(); i++) {
      big_tuple.add(next_tuple->get_pointer(i));
    }
    for (int i = 0; i < last_right_tuple.size(); i++) {
      big_tuple.add(last_right_tuple.get_pointer(i));
    }
    //投影操作
    Tuple projection_tuple;
    for (auto &f : plan_->OutputSchema()->fields()) {
      projection_tuple.add(
          f.expr()->Evaluate(&big_tuple, plan_->OutputSchema()));
    }
    // //输出
    tuple->operator=(std::move(projection_tuple));
    tuple->print(std::cout);  // debug
    return RC::SUCCESS;
  }

  // 2. probe
  std::string right_hashkey = plan_->GetRightColName();
  int col_index = right_hashkey == ""
                      ? -1
                      : right_->GetOutputSchema()->GetFieldIdx(right_hashkey);
  while (right_->Next(&last_right_tuple, rid) == RC::SUCCESS) {
    std::string s = "";

    if (col_index != -1) {
      std::stringstream ss;
      last_right_tuple.get(col_index).to_string(ss);
      ss >> s;

      std::cout << "probe: " << s << std::endl;
    }
    if (hash_map.count(s) > 0) {
      // std::cout << "[DEBUG] 左元组: ";
      // left_tuple->print(std::cout);
      // std::cout << "[DEBUG] 右元组: ";
      // last_right_tuple.print(std::cout);
      SetLastTuples(hash_map[s]);
      Tuple *left_tuple;
      bool is_matched = false;
      while (!is_matched) {
        //这个while如果没有JoinExpression，只做一次，相当于笔直向下走和原来是一样的
        //但是如果出现不匹配的元组，就会一直想方设法获取下一个匹配的
        left_tuple = GetNextTuple();
        if (left_tuple == nullptr) return RC::RECORD_EOF;
        is_matched = true;
        for (auto &p : plan_->JoinKeyExpression()) {
          if (p->EvaluateJoin(left_tuple, plan_->GetChildAt(0)->OutputSchema(),
                              &last_right_tuple,
                              plan_->GetChildAt(1)->OutputSchema())
                  ->compare(IntValue(true)) != 0) {
            is_matched = false;
            break;
          }
        }
      }

      //投影操作
      // Tuple projection_tuple;
      // for (auto &f : plan_->OutputSchema()->fields()) {
      //   projection_tuple.add(f.expr()->Evaluate(tmp_tuple,
      //   plan_->OutputSchema()));
      // }
      //组合
      Tuple big_tuple;
      for (int i = 0; i < left_tuple->size(); i++) {
        big_tuple.add(left_tuple->get_pointer(i));
      }
      for (int i = 0; i < last_right_tuple.size(); i++) {
        big_tuple.add(last_right_tuple.get_pointer(i));
      }
      std::cout << "[DEBUG] 组合元组: ";
      big_tuple.print(std::cout);
      //投影
      //投影操作
      Tuple projection_tuple;
      for (auto &f : plan_->OutputSchema()->fields()) {
        projection_tuple.add(
            f.expr()->Evaluate(&big_tuple, plan_->OutputSchema()));
      }
      std::cout << "[DEBUG] 投影schema: ";
      plan_->OutputSchema()->print(std::cout, true);
      std::cout << "[DEBUG] 投影后元组: ";
      projection_tuple.print(std::cout);
      // //输出
      tuple->operator=(std::move(projection_tuple));

      return RC::SUCCESS;
    }
  }
  return RC::RECORD_EOF;
}