#ifndef __OBSERVER_SQL_COLUMN_VALUE_EXPRESSION_H__
#define __OBSERVER_SQL_COLUMN_VALUE_EXPRESSION_H__
#include "sql/executor/expressions/abstract_expression.h"
#include "sql/executor/tuple.h"
#include "sql/executor/value.h"
class ColumnValueExpression : public AbstractExpression {
 public:
  /**
   * ColumnValueExpression is an abstraction around "Table.member" in terms of
   * indexes.
   * @param tuple_idx {tuple index 0 = left side of join, tuple index 1 = right
   * side of join}
   * @param col_idx the index of the column in the schema
   * @param ret_type the return type of the expression
   */
  ColumnValueExpression(size_t tuple_idx, size_t col_idx, AttrType ret_type)
      : AbstractExpression({}, ret_type),
        tuple_idx_{tuple_idx},
        col_idx_{col_idx} {}

  const std::shared_ptr<TupleValue> Evaluate(
      const Tuple *tuple, const TupleSchema *schema) const override {
    return tuple->get_pointer(col_idx_);
  }

  const std::shared_ptr<TupleValue> EvaluateJoin(
      const Tuple *left_tuple, const TupleSchema *left_schema,
      const Tuple *right_tuple,
      const TupleSchema *right_schema) const override {
    return tuple_idx_ == 0 ? left_tuple->get_pointer(col_idx_)
                           : right_tuple->get_pointer(col_idx_);
  }

  const std::shared_ptr<TupleValue> EvaluateAggregate(
      const std::vector<std::shared_ptr<TupleValue>> &group_bys,
      const std::vector<std::shared_ptr<TupleValue>> &aggregates)
      const override {
    assert(false &&
           "Aggregation should only refer to group-by and aggregates.");
  }

  size_t GetTupleIdx() const { return tuple_idx_; }
  size_t GetColIdx() const { return col_idx_; }
  void AppendColIdx(size_t delta) { col_idx_ += delta; }

 private:
  /** Tuple index 0 = left side of join, tuple index 1 = right side of join */
  size_t tuple_idx_;
  /** Column index refers to the index within the schema of the tuple, e.g.
   * schema {A,B,C} has indexes {0,1,2} */
  size_t col_idx_;
};
#endif