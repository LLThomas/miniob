#include "sql/executor/executors/aggregation_executor.h"
AggregationExecutor::AggregationExecutor(
    ExecutorContext *exec_ctx, const AggregationPlanNode *plan,
    std::unique_ptr<AbstractExecutor> &&child)
    : AbstractExecutor(exec_ctx),
      plan_(plan),
      child_(std::move(child)),
      aht_(plan->GetAggregates(), plan->GetAggregateTypes()),
      aht_iterator_(aht_.Begin()) {}

const AbstractExecutor *AggregationExecutor::GetChildExecutor() const {
  return child_.get();
}

void AggregationExecutor::Init() {
  child_->Init();
  RID rid;
  rid.page_num = 1;
  rid.slot_num = -1;
  Tuple tuple;
  while (child_->Next(&tuple, &rid) == RC::SUCCESS) {
    AggregateKey key = MakeKey(&tuple);
    std::cout << "[DEBUG] 哈希表 MakeKey: ";
    for (auto e : key.group_bys_) {
      e->to_string(std::cout);
      std::cout << " ";
    }
    std::cout << std::endl;

    AggregateValue value = MakeVal(&tuple);
    std::cout << "[DEBUG] 哈希表 MakeValue: ";
    for (auto e : value.aggregates_) {
      e->to_string(std::cout);
      std::cout << " ";
    }
    std::cout << std::endl;

    aht_.InsertCombine(key, value);

    std::cout << "[DEBUG] AGG值更新 : " << std::endl;
    for (SimpleAggregationHashTable::Iterator it = aht_.Begin();
         it != aht_.End(); it.operator++()) {
      std::cout << "( ";
      for (auto e : it.Key().group_bys_) {
        e->to_string(std::cout);
        std::cout << ",";
      }
      std::cout << ")";
      std::cout << "  ";
      for (auto e : it.Val().aggregates_) {
        e->to_string(std::cout);
        std::cout << " | ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  aht_iterator_ = aht_.Begin();
}

RC AggregationExecutor::Next(Tuple *tuple, RID *rid) {
  if (aht_iterator_ == aht_.End()) return RC::RECORD_EOF;
  Tuple big_tuple;
  AggregateKey key = aht_iterator_.Key();

  for (int i = 0; i < aht_iterator_.Val().aggregates_.size(); i++) {
    big_tuple.add(aht_iterator_.Val().aggregates_[i]);
  }
  for (auto k : key.group_bys_) {
    big_tuple.add(k);
  }
  //投影操作
  Tuple projection_tuple;
  for (auto &f : plan_->OutputSchema()->fields()) {
    projection_tuple.add(f.expr()->Evaluate(&big_tuple, plan_->OutputSchema()));
  }
  // //输出
  tuple->operator=(std::move(projection_tuple));
  tuple->print(std::cout);  // debug
  aht_iterator_.operator++();
  return RC::SUCCESS;
}
