//
// Created by thomas on 2021/11/17.
//

#include "sql/executor/executors/order_by_executor.h"
#include <algorithm>

OrderByExecutor::OrderByExecutor(
    ExecutorContext *exec_ctx, const OrderByPlanNode *plan,
    std::unique_ptr<AbstractExecutor> &&child)
    : AbstractExecutor(exec_ctx),
      plan_(plan),
      child_(std::move(child)) {}

std::vector<std::pair<std::string, int>> order_bys;
TupleSchema *schema;

bool OrderByExecutor::cmp(Tuple *tuple_1, Tuple *tuple_2) {
  for (int i = 0; i < order_bys.size(); i++) {
    int idx = schema->GetColIdx(order_bys[i].first);
    if (tuple_1->get(idx).compare(tuple_2->get(idx)) < 0) {
      return order_bys[i].second == 1;
    } else if (tuple_1->get(idx).compare(tuple_2->get(idx)) > 0) {
      return order_bys[i].second == 0;
    } else {
      continue;
    }
  }
  return true;
}

void OrderByExecutor::Init() {
  order_bys = GetOrderBys();
  schema = GetOutputSchema();
  std::vector<Tuple *> tuple_set = GetTuples();
  RID rid;
  rid.page_num = 1;
  rid.slot_num = -1;
  Tuple tuple;
  // construct tuple_set
  while (child_->Next(&tuple, &rid) == RC::SUCCESS) {
    tuple_set.emplace_back(&tuple);
  }
  // sort by order_bys
  std::sort(tuple_set.begin(), tuple_set.end(), cmp);
}

RC OrderByExecutor::Next(Tuple *tuple, RID *rid) {
  std::vector<Tuple *> tuples = GetTuples();
  while (tuples.size() > 0) {
    tuple = tuples.front();
    tuples.erase(tuples.begin());
    return RC::SUCCESS;
  }
  return RC::RECORD_EOF;
}