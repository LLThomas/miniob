
#ifndef __OBSERVER_SQL_CONSTANT_VALUE_EXPRESSION_H__
#define __OBSERVER_SQL_CONSTANT_VALUE_EXPRESSION_H__

#include <vector>

#include "sql/executor/expressions/abstract_expression.h"
#include "sql/executor/tuple.h"
#include "sql/executor/value.h"
/**
 * ConstantValueExpression represents constants.
 */
class ConstantValueExpression : public AbstractExpression {
 public:
  /** Creates a new constant value expression wrapping the given value. */
  explicit ConstantValueExpression(const std::shared_ptr<TupleValue> &val)
      : AbstractExpression({}, val->get_type()), val_(val) {}

  const std::shared_ptr<TupleValue> Evaluate(
      const Tuple *tuple, const TupleSchema *schema) const override {
    return val_;
  }

  const std::shared_ptr<TupleValue> EvaluateJoin(
      const Tuple *left_tuple, const TupleSchema *left_schema,
      const Tuple *right_tuple,
      const TupleSchema *right_schema) const override {
    return val_;
  }

  const std::shared_ptr<TupleValue> EvaluateAggregate(
      const std::vector<TupleValue> &group_bys,
      const std::vector<TupleValue> &aggregates) const override {
    return val_;
  }

 private:
  std::shared_ptr<TupleValue> val_;
};
#endif