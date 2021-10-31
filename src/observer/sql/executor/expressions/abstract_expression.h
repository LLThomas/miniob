#ifndef __OBSERVER_SQL_ABSTRACT_EXPRESSION_H__
#define __OBSERVER_SQL_ABSTRACT_EXPRESSION_H__
#include <vector>

#include "sql/executor/value.h"
#include "sql/parser/parse.h"
class Tuple;
class TupleSchema;

class AbstractExpression {
 public:
  AbstractExpression(std::vector<const AbstractExpression *> &&children,
                     AttrType ret_type)
      : children_{std::move(children)}, ret_type_{ret_type} {}

  virtual ~AbstractExpression() = default;

  virtual const std::shared_ptr<TupleValue> &Evaluate(
      const Tuple *tuple, const TupleSchema *schema) const = 0;

  virtual const std::shared_ptr<TupleValue> &EvaluateJoin(
      const Tuple *left_tuple, const TupleSchema *left_schema,
      const Tuple *right_tuple, const TupleSchema *right_schema) const = 0;

  virtual const std::shared_ptr<TupleValue> &EvaluateAggregate(
      const std::vector<TupleValue> &group_bys,
      const std::vector<TupleValue> &aggregates) const = 0;

  const AbstractExpression *GetChildAt(size_t child_idx) const {
    return children_[child_idx];
  }

  /** @return the children of this expression, ordering may matter */
  const std::vector<const AbstractExpression *> &GetChildren() const {
    return children_;
  }

  /** @return the type of this expression if it were to be evaluated */
  virtual AttrType GetReturnType() const { return ret_type_; }

 private:
  /** The children of this expression. Note that the order of appearance of
   * children may matter. */
  std::vector<const AbstractExpression *> children_;
  /** The return type of this expression. */
  AttrType ret_type_;
};
#endif