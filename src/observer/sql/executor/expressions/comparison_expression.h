
// #ifndef __OBSERVER_SQL_COMPARISON_EXPRESSION_H__
// #define __OBSERVER_SQL_COMPARISON_EXPRESSION_H__

// #include <utility>
// #include <vector>

// #include "executor/expressions/abstract_expression.h"

// /** ComparisonType represents the type of comparison that we want to perform.
// */ enum class ComparisonType {
//   Equal,
//   NotEqual,
//   LessThan,
//   LessThanOrEqual,
//   GreaterThan,
//   GreaterThanOrEqual
// };

// /**
//  * ComparisonExpression represents two expressions being compared.
//  */
// class ComparisonExpression : public AbstractExpression {
//  public:
//   /** Creates a new comparison expression representing (left comp_type
//   right).
//    */
//   ComparisonExpression(const AbstractExpression *left,
//                        const AbstractExpression *right,
//                        ComparisonType comp_type)
//       : AbstractExpression({left, right}, TypeId::BOOLEAN),
//         comp_type_{comp_type} {}

//   TupleValue Evaluate(const Tuple *tuple, const Schema *schema) const
//   override {
//     Value lhs = GetChildAt(0)->Evaluate(tuple, schema);
//     Value rhs = GetChildAt(1)->Evaluate(tuple, schema);
//     return ValueFactory::GetBooleanValue(PerformComparison(lhs, rhs));
//   }

//   TupleValue EvaluateJoin(const Tuple *left_tuple, const Schema *left_schema,
//                           const Tuple *right_tuple,
//                           const Schema *right_schema) const override {
//     Value lhs = GetChildAt(0)->EvaluateJoin(left_tuple, left_schema,
//                                             right_tuple, right_schema);
//     Value rhs = GetChildAt(1)->EvaluateJoin(left_tuple, left_schema,
//                                             right_tuple, right_schema);
//     return ValueFactory::GetBooleanValue(PerformComparison(lhs, rhs));
//   }

//   TupleValue EvaluateAggregate(
//       const std::vector<Value> &group_bys,
//       const std::vector<Value> &aggregates) const override {
//     Value lhs = GetChildAt(0)->EvaluateAggregate(group_bys, aggregates);
//     Value rhs = GetChildAt(1)->EvaluateAggregate(group_bys, aggregates);
//     return ValueFactory::GetBooleanValue(PerformComparison(lhs, rhs));
//   }

//  private:
//   CmpBool PerformComparison(const Value &lhs, const Value &rhs) const {
//     switch (comp_type_) {
//       case ComparisonType::Equal:
//         return lhs.CompareEquals(rhs);
//       case ComparisonType::NotEqual:
//         return lhs.CompareNotEquals(rhs);
//       case ComparisonType::LessThan:
//         return lhs.CompareLessThan(rhs);
//       case ComparisonType::LessThanOrEqual:
//         return lhs.CompareLessThanEquals(rhs);
//       case ComparisonType::GreaterThan:
//         return lhs.CompareGreaterThan(rhs);
//       case ComparisonType::GreaterThanOrEqual:
//         return lhs.CompareGreaterThanEquals(rhs);
//       default:
//         BUSTUB_ASSERT(false, "Unsupported comparison type.");
//     }
//   }

//   std::vector<const AbstractExpression *> children_;
//   ComparisonType comp_type_;
// };
// #endif