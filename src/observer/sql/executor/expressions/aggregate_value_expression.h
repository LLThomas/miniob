#ifndef __OBSERVER_SQL_AGGREGATE_VALUE_EXPRESSION_H__
#define __OBSERVER_SQL_AGGREGATE_VALUE_EXPRESSION_H__
#include "sql/executor/expressions/abstract_expression.h"
#include "sql/executor/tuple.h"
#include "sql/executor/value.h"
class AggregateValueExpression : public AbstractExpression {
 public:
  /**
   * Creates a new AggregateValueExpression.
   * @param is_group_by_term true if this is a group by
   * @param term_idx the index of the term
   * @param ret_type the return type of the aggregate value expression
   */
  AggregateValueExpression(bool is_group_by_term, uint32_t term_idx,
                           TypeId ret_type)
      : AbstractExpression({}, ret_type),
        is_group_by_term_{is_group_by_term},
        term_idx_{term_idx} {}

  Value Evaluate(const Tuple *tuple, const Schema *schema) const override {
    BUSTUB_ASSERT(false,
                  "Aggregation should only refer to group-by and aggregates.");
  }

  Value EvaluateJoin(const Tuple *left_tuple, const Schema *left_schema,
                     const Tuple *right_tuple,
                     const Schema *right_schema) const override {
    BUSTUB_ASSERT(false,
                  "Aggregation should only refer to group-by and aggregates.");
  }

  Value EvaluateAggregate(const std::vector<Value> &group_bys,
                          const std::vector<Value> &aggregates) const override {
    return is_group_by_term_ ? group_bys[term_idx_] : aggregates[term_idx_];
  }

 private:
  bool is_group_by_term_;
  uint32_t term_idx_;
};
#endif