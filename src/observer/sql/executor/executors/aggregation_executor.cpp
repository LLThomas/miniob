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

void AggregationExecutor::Init() {}

RC AggregationExecutor::Next(Tuple *tuple, RID *rid) { return RC::SUCCESS; }
