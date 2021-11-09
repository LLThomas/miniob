
#ifndef __OBSERVER_SQL_COMPARISON_EXPRESSION_H__
#define __OBSERVER_SQL_COMPARISON_EXPRESSION_H__

#include <iostream>
#include <utility>
#include <vector>

#include "sql/executor/expressions/abstract_expression.h"
#include "sql/executor/tuple.h"
#include "sql/executor/value.h"

/**
 * ComparisonExpression represents two expressions being compared.
 */
class ComparisonExpression : public AbstractExpression {
 public:
  /** Creates a new comparison expression representing (left comp_type
  right).
   */
  ComparisonExpression(const AbstractExpression *left,
                       const AbstractExpression *right, CompOp comp_type)
      : AbstractExpression({left, right}, AttrType::INTS),
        comp_type_{comp_type} {}

  const std::shared_ptr<TupleValue> Evaluate(
      const Tuple *tuple, const TupleSchema *schema) const override {
    std::shared_ptr<TupleValue> lhs = GetChildAt(0)->Evaluate(tuple, schema);
    std::shared_ptr<TupleValue> rhs = nullptr;
    if (comp_type_ != IS_LEFT_NULL && comp_type_ != IS_LEFT_NOT_NULL) {
      rhs = GetChildAt(1)->Evaluate(tuple, schema);
    }
    return std::make_shared<IntValue>(PerformComparison(lhs, rhs));
  }

  const std::shared_ptr<TupleValue> EvaluateJoin(
      const Tuple *left_tuple, const TupleSchema *left_schema,
      const Tuple *right_tuple,
      const TupleSchema *right_schema) const override {
    std::shared_ptr<TupleValue> lhs = GetChildAt(0)->EvaluateJoin(
        left_tuple, left_schema, right_tuple, right_schema);
    std::shared_ptr<TupleValue> rhs = GetChildAt(1)->EvaluateJoin(
        left_tuple, left_schema, right_tuple, right_schema);
    return std::make_shared<IntValue>(PerformComparison(lhs, rhs));
  }

  const std::shared_ptr<TupleValue> EvaluateAggregate(
      const std::vector<TupleValue> &group_bys,
      const std::vector<TupleValue> &aggregates) const override {
    std::shared_ptr<TupleValue> lhs =
        GetChildAt(0)->EvaluateAggregate(group_bys, aggregates);
    std::shared_ptr<TupleValue> rhs =
        GetChildAt(1)->EvaluateAggregate(group_bys, aggregates);
    return std::make_shared<IntValue>(PerformComparison(lhs, rhs));
  }

 private:
  int PerformComparison(const std::shared_ptr<TupleValue> &lhs,
                        const std::shared_ptr<TupleValue> &rhs) const {
    if (comp_type_ != IS_LEFT_NULL && comp_type_ != IS_LEFT_NOT_NULL &&
        (std::dynamic_pointer_cast<NullValue>(lhs) != nullptr ||
         std::dynamic_pointer_cast<NullValue>(rhs) != nullptr)) {
      return 0;
    }
    switch (comp_type_) {
      case CompOp::EQUAL_TO:
        return lhs->compare(*rhs) == 0;
      case CompOp::NOT_EQUAL:
        return lhs->compare(*rhs) != 0;
      case CompOp::LESS_THAN:
        return lhs->compare(*rhs) < 0;
      case CompOp::LESS_EQUAL:
        return lhs->compare(*rhs) <= 0;
      case CompOp::GREAT_THAN:
        return lhs->compare(*rhs) > 0;
      case CompOp::GREAT_EQUAL:
        return lhs->compare(*rhs) >= 0;
      case CompOp::IS_LEFT_NULL:
        return std::dynamic_pointer_cast<NullValue>(lhs) != nullptr;
      case CompOp::IS_LEFT_NOT_NULL:
        return std::dynamic_pointer_cast<NullValue>(lhs) == nullptr;
      default:
        assert(false && "Unsupported comparison type.");
    }
  }

  std::vector<const AbstractExpression *> children_;
  CompOp comp_type_;
};
#endif