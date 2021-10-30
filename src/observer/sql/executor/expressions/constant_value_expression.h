
#pragma once

#include <vector>

#include "execution/expressions/abstract_expression.h"

/**
 * ConstantValueExpression represents constants.
 */
class ConstantValueExpression : public AbstractExpression {
 public:
  /** Creates a new constant value expression wrapping the given value. */
  explicit ConstantValueExpression(const Value &val)
      : AbstractExpression({}, val.GetTypeId()), val_(val) {}

  TupleValue Evaluate(const Tuple *tuple, const Schema *schema) const override {
    return val_;
  }

  TupleValue EvaluateJoin(const Tuple *left_tuple, const Schema *left_schema,
                          const Tuple *right_tuple,
                          const Schema *right_schema) const override {
    return val_;
  }

  TupleValue EvaluateAggregate(
      const std::vector<Value> &group_bys,
      const std::vector<Value> &aggregates) const override {
    return val_;
  }

 private:
  Value val_;
};