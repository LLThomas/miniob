#ifndef __OBSERVER_SQL_HASH_JOIN_PLAN_H__
#define __OBSERVER_SQL_HASH_JOIN_PLAN_H__
#include "sql/executor/plans/abstract_plan.h"
#include "sql/executor/tuple.h"

/**
 * Hash join performs a JOIN operation with a hash table.
 */
class HashJoinPlanNode : public AbstractPlanNode {
 public:
  /**
   * Construct a new HashJoinPlanNode instance.
   * @param output_schema The output schema for the JOIN
   * @param children The child plans from which tuples are obtained
   * @param left_key_expression The expression for the left JOIN key
   * @param right_key_expression The expression for the right JOIN key
   */
  HashJoinPlanNode(TupleSchema *output_schema, std::vector<AbstractPlanNode *> &&children,
                   const AbstractExpression *left_key_expression,
                   const AbstractExpression *right_key_expression,
                   std::string left_col_name,
                   std::string right_col_name)
      : AbstractPlanNode(output_schema, std::move(children)),
        left_key_expression_{left_key_expression},
        right_key_expression_{right_key_expression},
        left_col_name(left_col_name),
        right_col_name(right_col_name) {}

  /** @return The type of the plan node */
  PlanType GetType() const override { return PlanType::HashJoin; }

  /** @return The expression to compute the left join key */
  const AbstractExpression *LeftJoinKeyExpression() const { return left_key_expression_; }

  /** @return The expression to compute the right join key */
  const AbstractExpression *RightJoinKeyExpression() const { return right_key_expression_; }

  /** @return The left plan node of the hash join */
  AbstractPlanNode *GetLeftPlan() const {
    // BUSTUB_ASSERT(GetChildren().size() == 2, "Hash joins should have exactly two children plans.");
    return GetChildAt(0);
  }

  /** @return The right plan node of the hash join */
  AbstractPlanNode *GetRightPlan() const {
    // BUSTUB_ASSERT(GetChildren().size() == 2, "Hash joins should have exactly two children plans.");
    return GetChildAt(1);
  }

  const std::string GetLeftColName() {
    return left_col_name;
  }

  const std::string GetRightColName() {
    return right_col_name;
  }

 private:
  /** The expression to compute the left JOIN key */
  const AbstractExpression *left_key_expression_;
  /** The expression to compute the right JOIN key */
  const AbstractExpression *right_key_expression_;

  std::string left_col_name;
  std::string right_col_name;
};

#endif