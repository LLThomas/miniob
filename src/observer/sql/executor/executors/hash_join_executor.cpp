#include "sql/executor/executors/hash_join_executor.h"

#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::vector<Tuple *>> hash_map;

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
  Tuple tuple;
  RID rid;
  rid.page_num = 1;
  rid.slot_num = -1;
  while (left_->Next(&tuple, &rid) == RC::SUCCESS) {
    std::stringstream ss;
    tuple.get(col_index).to_string(ss);
    std::string s;
    ss >> s;

    std::cout << "build: " << s << std::endl;

    hash_map[s].emplace_back(&tuple);
  }
  LOG_ERROR("break\n");
  right_->Init();
}

RC HashJoinExecutor::Next(Tuple *tuple, RID *rid) {
  LOG_ERROR("right Next\n");

  // check last_tuples first
  Tuple *next_tuple = GetNextTuple();
  if (next_tuple != nullptr) {
    // TODO: 组合+返回
    return RC::SUCCESS;
  }

  // 2. probe
  int col_index =
      right_->GetOutputSchema()->GetColIdx(plan_->GetRightColName());
  while (right_->Next(tuple, rid) == RC::SUCCESS) {
    std::stringstream ss;
    tuple->get(col_index).to_string(ss);
    std::string s;
    ss >> s;

    std::cout << "probe: " << s << std::endl;

    if (hash_map.find(s) != hash_map.end()) {
      SetLastTuples(hash_map[s]);
      Tuple *tmp_tuple = GetNextTuple();

      //投影操作
      Tuple projection_tuple;
      for (auto &f : plan_->OutputSchema()->fields()) {
        projection_tuple.add(f.expr()->Evaluate(tuple, plan_->OutputSchema()));
      }
      // //输出
      tuple->operator=(std::move(projection_tuple));
      tuple->print(std::cout);  // debug

      return RC::SUCCESS;
    }
  }
  return RC::EMPTY;
}