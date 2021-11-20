
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
    // std::cout << "[DEBUG] 比较表达式：";
    // GetChildAt(0)->Evaluate(tuple, nullptr)->to_string(std::cout);
    // std::cout << " " << GetCompOp() << " ";
    // GetChildAt(1)->Evaluate(tuple, nullptr)->to_string(std::cout);
    // std::cout << std::endl;
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
      std::vector<std::shared_ptr<TupleValue>> &group_bys,
      std::vector<std::shared_ptr<TupleValue>> &aggregates) const override {
    std::shared_ptr<TupleValue> lhs =
        GetChildAt(0)->EvaluateAggregate(group_bys, aggregates);
    std::shared_ptr<TupleValue> rhs =
        GetChildAt(1)->EvaluateAggregate(group_bys, aggregates);
    return std::make_shared<IntValue>(PerformComparison(lhs, rhs));
  }
  const CompOp GetCompOp() const { return comp_type_; }

 private:
  int PerformComparison(const std::shared_ptr<TupleValue> &lhs,
                        const std::shared_ptr<TupleValue> &rhs) const {
    if (comp_type_ != IS_LEFT_NULL && comp_type_ != IS_LEFT_NOT_NULL &&
        (std::dynamic_pointer_cast<NullValue>(lhs) != nullptr ||
         std::dynamic_pointer_cast<NullValue>(lhs) != nullptr)) {
      return 0;
    }
    TupleValue *l = lhs.get(), *r = rhs.get();
    if (lhs->get_type() == AttrType::INTS) {
      l = new FloatValue((float)((IntValue *)lhs.get())->get_value());
    }
    if (rhs->get_type() == AttrType::INTS) {
      r = new FloatValue((float)((IntValue *)rhs.get())->get_value());
    }
    switch (comp_type_) {
      case CompOp::EQUAL_TO:
        return l->compare(*r) == 0;
      case CompOp::NOT_EQUAL:
        return l->compare(*r) != 0;
      case CompOp::LESS_THAN:
        return l->compare(*r) < 0;
      case CompOp::LESS_EQUAL:
        return l->compare(*r) <= 0;
      case CompOp::GREAT_THAN:
        return l->compare(*r) > 0;
      case CompOp::GREAT_EQUAL:
        return l->compare(*r) >= 0;
      case CompOp::IS_LEFT_NULL:
        return lhs->get_type() == AttrType::NULLS;
      case CompOp::IS_LEFT_NOT_NULL:
        return lhs->get_type() != AttrType::NULLS;
      default:
        assert(false && "Unsupported comparison type.");
    }
  }

  std::vector<const AbstractExpression *> children_;
  CompOp comp_type_;
};
#endif