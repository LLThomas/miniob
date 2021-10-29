
#pragma once
#include <vector>

#include "sql/executor/tuple.h"
enum class PlanType { SeqScan };

class AbstractPlanNode {
 public:
  AbstractPlanNode(TupleSchema *output_schema,
                   std::vector<const AbstractPlanNode *> &&children)
      : output_schema_(output_schema), children_(std::move(children)) {}

  virtual ~AbstractPlanNode() = default;

  TupleSchema *OutputSchema() const { return output_schema_; }

  const AbstractPlanNode *GetChildAt(int child_idx) const {
    return children_[child_idx];
  }

  const std::vector<const AbstractPlanNode *> &GetChildren() const {
    return children_;
  }

  virtual PlanType GetType() const = 0;

 private:
  TupleSchema *output_schema_;
  std::vector<const AbstractPlanNode *> children_;
};