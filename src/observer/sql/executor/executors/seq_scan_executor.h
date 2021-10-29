#pragma once
#include "sql/executor/executor_context.h"
#include "sql/executor/executors/abstract_executor.h"
#include "sql/executor/plans/seq_scan_plan.h"

class SeqScanExecutor : public AbstractExecutor {
 public:
  SeqScanExecutor(ExecutorContext *exec_ctx, SeqScanPlanNode *plan);

  void Init() override;

  RC Next(Tuple *tuple, RID *rid) override;

  TupleSchema *GetOutputSchema() override { return plan_->OutputSchema(); }

 private:
  SeqScanPlanNode *plan_;
};