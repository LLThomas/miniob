//
// Created by thomas on 2021/11/17.
//

#include "sql/executor/plans/abstract_plan.h"

#ifndef MINIDB_ORDER_BY_PLAN_H
#define MINIDB_ORDER_BY_PLAN_H

class OrderByPlanNode : public AbstractPlanNode {
 public:
  OrderByPlanNode(TupleSchema *output_schema, AbstractPlanNode *child,
                  std::vector<std::pair<std::string, int>> order_bys)
      : AbstractPlanNode(output_schema, {child}),
        order_bys_(order_bys) {}

  std::vector<std::pair<std::string, int>> GetOrderBys() const {
    return order_bys_;
  }

  PlanType GetType() const override { return PlanType::OrderBy; }

  /** @return the child of this order by plan node */
  AbstractPlanNode *GetChildPlan() const {
    assert(GetChildren().size() == 1 &&
           "Order By expected to only have one child.");
    return GetChildAt(0);
  }

 private:
  std::vector<std::pair<std::string, int>> order_bys_;
};

#endif  // MINIDB_ORDER_BY_PLAN_H
