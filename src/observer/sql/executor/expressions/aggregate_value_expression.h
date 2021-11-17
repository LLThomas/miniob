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
  AggregateValueExpression(bool is_group_by_term, int term_idx,
                           AttrType ret_type)
      : AbstractExpression({}, ret_type),
        is_group_by_term_{is_group_by_term},
        term_idx_{term_idx} {}

  const std::shared_ptr<TupleValue> Evaluate(
      const Tuple *tuple, const TupleSchema *schema) const override {
    assert(false &&
           "Aggregation should only refer to group-by and aggregates.");
  }

  const std::shared_ptr<TupleValue> EvaluateJoin(
      const Tuple *left_tuple, const TupleSchema *left_schema,
      const Tuple *right_tuple,
      const TupleSchema *right_schema) const override {
    assert(false &&
           "Aggregation should only refer to group-by and aggregates.");
  }

  const std::shared_ptr<TupleValue> EvaluateAggregate(
      std::vector<std::shared_ptr<TupleValue>> &group_bys,
      std::vector<std::shared_ptr<TupleValue>> &aggregates) const override {
    if (is_group_by_term_)
      return group_bys[term_idx_];
    else
      return aggregates[term_idx_];
  }
  bool GetIsGroupBy() const { return is_group_by_term_; }
  uint32_t GetTermIdx() const { return term_idx_; }

 private:
  bool is_group_by_term_;
  int term_idx_;
};
#endif