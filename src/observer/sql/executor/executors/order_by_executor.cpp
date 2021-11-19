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
    int idx = schema->GetFieldIdx(order_bys[i].first);
    if (tuple_1->get(idx).compare(tuple_2->get(idx)) < 0) {
      if (order_bys[i].second == 1) {
        return true;
      }
      return false;
    } else if (tuple_1->get(idx).compare(tuple_2->get(idx)) > 0) {
      if (order_bys[i].second == 0) {
        return true;
      }
      return false;
    }
  }
  return true;
}

void OrderByExecutor::Init() {
  child_->Init();
  order_bys = GetOrderBys();
  schema = GetOutputSchema();
  std::vector<Tuple *> tuple_set;
  RID rid;
  rid.page_num = 1;
  rid.slot_num = -1;
  Tuple tuple;
  // construct tuple_set
  while (child_->Next(&tuple, &rid) == RC::SUCCESS) {
    Tuple *next_tuple = new Tuple();
    for (int i = 0; i < tuple.size(); i++) {
      next_tuple->add(tuple.get_pointer(i));
    }
    tuple_set.push_back(next_tuple);
  }
  // sort by order_bys
  std::sort(tuple_set.begin(), tuple_set.end(), cmp);
  SetTuples(tuple_set);
}

RC OrderByExecutor::Next(Tuple *tuple, RID *rid) {
  while (!GetTuples().empty()) {
    Tuple *next_tuple = GetOneTuple();
    tuple->clear();
    for (int i = 0; i < next_tuple->size(); i++) {
      tuple->add(next_tuple->get_pointer(i));
    }
    return RC::SUCCESS;
  }
  return RC::RECORD_EOF;
}