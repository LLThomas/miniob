
#ifndef __OBSERVER_SQL_ABSTRACT_PLAN_H__
#define __OBSERVER_SQL_ABSTRACT_PLAN_H__
#include <vector>

#include "sql/executor/tuple.h"
enum class PlanType { SeqScan, HashJoin, Aggregation };

class AbstractPlanNode {
 public:
  AbstractPlanNode(TupleSchema *output_schema,
                   std::vector<AbstractPlanNode *> &&children)
      : output_schema_(output_schema), children_(std::move(children)) {}

  virtual ~AbstractPlanNode() = default;

  TupleSchema *OutputSchema() const { return output_schema_; }
  void SetOutputSchema(TupleSchema *schema) { output_schema_ = schema; }
  AbstractPlanNode *GetChildAt(int child_idx) const {
    return children_[child_idx];
  }

  const std::vector<AbstractPlanNode *> &GetChildren() const {
    return children_;
  }

  virtual PlanType GetType() const = 0;

 private:
  TupleSchema *output_schema_;
  std::vector<AbstractPlanNode *> children_;
};
#endif