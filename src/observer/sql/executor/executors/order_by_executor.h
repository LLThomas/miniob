//
// Created by thomas on 2021/11/17.
//

#include "sql/executor/executor_context.h"
#include "sql/executor/executors/abstract_executor.h"
#include "sql/executor/plans/order_by_plan.h"

#ifndef MINIDB_ORDER_BY_EXECUTOR_H
#define MINIDB_ORDER_BY_EXECUTOR_H

class OrderByExecutor : public AbstractExecutor {
 public:
  OrderByExecutor(ExecutorContext *exec_ctx,
                  const OrderByPlanNode *plan,
                  std::unique_ptr<AbstractExecutor> &&child);

  TupleSchema *GetOutputSchema() override { return plan_->OutputSchema(); };

  void SetTuples(std::vector<Tuple *> tuples) {
    tuples_ = tuples;
  }

  Tuple *GetOneTuple() {
    Tuple *tuple = tuples_.front();
    tuples_.erase(tuples_.begin());
    return tuple;
  }

  std::vector<Tuple *> GetTuples() {
    return tuples_;
  }

  std::vector<std::pair<std::string, int>> GetOrderBys() {
    return plan_->GetOrderBys();
  }

  static bool cmp(Tuple *tuple_1, Tuple *tuple_2) ;

  void Init() override;

  RC Next(Tuple *tuple, RID *rid) override;

 private:
  const OrderByPlanNode *plan_;
  std::unique_ptr<AbstractExecutor> child_;
  std::vector<Tuple *> tuples_;
};

#endif  // MINIDB_ORDER_BY_EXECUTOR_H
