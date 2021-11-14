#include "sql/executor/executors/hash_join_executor.h"

#include <string>
#include <unordered_map>

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
  std::string on_col_name = plan_->GetLeftColName();
  int col_index = left_->GetOutputSchema()->GetColIdx(on_col_name);
  Tuple *tuple = new Tuple();
  RID rid;
  rid.page_num = 1;
  rid.slot_num = -1;
  while (left_->Next(tuple, &rid) == RC::SUCCESS) {
    std::stringstream ss;
    tuple->get(col_index).to_string(ss);
    std::string s;
    ss >> s;

    std::cout << "build: " << s << std::endl;

    hash_map[s].emplace_back(tuple);
  }
  LOG_ERROR("break\n");
  right_->Init();
}

RC HashJoinExecutor::Next(Tuple *tuple, RID *rid) {
  LOG_ERROR("right Next\n");

  // check last_tuples first
  Tuple *next_tuple = GetNextTuple();
  if (next_tuple != nullptr) {
    //组合
    Tuple big_tuple;
    for (int i = 0; i < next_tuple->size(); i++) {
      big_tuple.add(next_tuple->get_pointer(i));
    }
    for (int i = 0; i < last_right_tuple.size(); i++) {
      big_tuple.add(last_right_tuple.get_pointer(i));
    }
    // //输出
    tuple->operator=(std::move(big_tuple));
    tuple->print(std::cout);  // debug
    return RC::SUCCESS;
  }

  // 2. probe
  int col_index =
      right_->GetOutputSchema()->GetColIdx(plan_->GetRightColName());
  while (right_->Next(&last_right_tuple, rid) == RC::SUCCESS) {
    std::stringstream ss;
    last_right_tuple.get(col_index).to_string(ss);
    std::string s;
    ss >> s;

    std::cout << "probe: " << s << std::endl;

    if (hash_map.count(s) > 0) {
      SetLastTuples(hash_map[s]);
      Tuple *left_tuple = GetNextTuple();
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
      // //输出
      tuple->operator=(std::move(big_tuple));
      tuple->print(std::cout);  // debug

      return RC::SUCCESS;
    }
  }
  return RC::RECORD_EOF;
}