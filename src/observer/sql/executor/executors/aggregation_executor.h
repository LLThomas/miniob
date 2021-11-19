
#include <climits>
#include <limits>
#include <unordered_map>
#include <vector>

#include "sql/executor/executor_context.h"
#include "sql/executor/executors/abstract_executor.h"
#include "sql/executor/expressions/abstract_expression.h"
#include "sql/executor/expressions/aggregate_value_expression.h"
#include "sql/executor/expressions/column_value_expression.h"
#include "sql/executor/plans/aggregation_plan.h"
#include "storage/common/hash_util.h"
/**
 * A simplified hash table that has all the necessary functionality for
 * aggregations.
 */
class SimpleAggregationHashTable {
 public:
  /**
   * Create a new simplified aggregation hash table.
   * @param agg_exprs the aggregation expressions
   * @param agg_types the types of aggregations
   */
  SimpleAggregationHashTable(
      const std::vector<const AbstractExpression *> &agg_exprs,
      const std::vector<AggregationType> &agg_types)
      : agg_exprs_{agg_exprs}, agg_types_{agg_types} {}

  /** @return the initial aggregate value for this aggregation executor */
  AggregateValue GenerateInitialAggregateValue() {
    std::vector<std::shared_ptr<TupleValue>> values;
    for (const auto &agg_type : agg_types_) {
      switch (agg_type) {
        case AggregationType::CountAggregate:
          // Count starts at zero.
          values.emplace_back(std::make_shared<IntValue>(0));
          break;
        case AggregationType::AvgAggregate:
          // Sum starts at zero.
          values.emplace_back(std::make_shared<NullValue>());
          break;
        case AggregationType::MinAggregate:
          // Min starts at INT_MAX.
          values.emplace_back(std::make_shared<NullValue>());
          break;
        case AggregationType::MaxAggregate:
          // Max starts at INT_MIN.
          values.emplace_back(std::make_shared<NullValue>());
          break;
      }
    }
    return {values};
  }

  /** Combines the input into the aggregation result. */
  void CombineAggregateValues(AggregateValue *result,
                              const AggregateValue &input,
                              const AggregateKey &key) {
    // for (int i = 0; i < result->aggregates_.size(); i++) {
    //   result->aggregates_[i]->to_string(std::cout);
    //   std::cout << " " << std::endl;
    // }
    // for (int i = 0; i < input.aggregates_.size(); i++) {
    //   input.aggregates_[i]->to_string(std::cout);
    //   std::cout << " " << std::endl;
    // }
    for (uint32_t i = 0; i < agg_exprs_.size(); i++) {
      std::shared_ptr<TupleValue> result_ptr = result->aggregates_[i];
      std::shared_ptr<TupleValue> input_ptr = input.aggregates_[i];
      if (input_ptr->get_type() == AttrType::NULLS) continue;
      switch (agg_types_[i]) {
        case AggregationType::CountAggregate: {  // Count increases by one.
          int old_cnt = ((IntValue *)result_ptr.get())->get_value();
          result_ptr->set_value(IntValue(old_cnt + 1));
          break;
        }
        case AggregationType::AvgAggregate: {
          // Sum increases by addition.
          if (avg_map_.count(key) == 0) {
            avg_map_[key] = std::unordered_map<int, std::pair<float, int>>{};
          }
          if (avg_map_[key].count(i) == 0) {
            avg_map_[key][i] = {0.0, 0};
            FloatValue f(0.0);
            result->aggregates_[i] = f.copy();
            result_ptr = result->aggregates_[i];
          }
          float &old_sum = avg_map_[key][i].first;
          int &old_cnt = avg_map_[key][i].second;
          if (input_ptr->get_type() == AttrType::INTS) {
            old_sum += ((IntValue *)input_ptr.get())->get_value();
          }
          if (input_ptr->get_type() == AttrType::FLOATS) {
            old_sum += ((FloatValue *)input_ptr.get())->get_value();
          }
          old_cnt += 1;
          result_ptr->set_value(FloatValue(old_sum / old_cnt));
          break;
        }

        case AggregationType::MinAggregate: {
          if (result_ptr->get_type() == AttrType::NULLS) {
            result->aggregates_[i] = input_ptr->copy();
          } else {
            result_ptr->set_value(input_ptr.get()->compare(*result_ptr) < 0
                                      ? *input_ptr
                                      : *result_ptr);
          }
          break;
        }
        case AggregationType::MaxAggregate: {
          if (result_ptr->get_type() == AttrType::NULLS) {
            result->aggregates_[i] = input_ptr->copy();
          } else {
            result_ptr->set_value(input_ptr.get()->compare(*result_ptr) > 0
                                      ? *input_ptr
                                      : *result_ptr);
          }
          break;
        }
      }
    }
  }

  /**
   * Inserts a value into the hash table and then combines it with the
   current
   * aggregation.
   * @param agg_key the key to be inserted
   * @param agg_val the value to be inserted
   */
  void InsertCombine(const AggregateKey &agg_key,
                     const AggregateValue &agg_val) {
    // for (auto a : agg_key.group_bys_) {
    //   a->to_string(std::cout);
    // }
    if (ht_.count(agg_key) == 0) {
      ht_.insert({agg_key, GenerateInitialAggregateValue()});
    }
    CombineAggregateValues(&ht_[agg_key], agg_val, agg_key);
  }

  /**
   * An iterator through the simplified aggregation hash table.
   */
  class Iterator {
   public:
    /** Creates an iterator for the aggregate map. */
    explicit Iterator(
        std::unordered_map<AggregateKey, AggregateValue>::const_iterator iter)
        : iter_(iter) {}

    /** @return the key of the iterator */
    const AggregateKey &Key() { return iter_->first; }

    /** @return the value of the iterator */
    const AggregateValue &Val() { return iter_->second; }

    /** @return the iterator before it is incremented */
    Iterator &operator++() {
      ++iter_;
      return *this;
    }

    /** @return true if both iterators are identical */
    bool operator==(const Iterator &other) {
      return this->iter_ == other.iter_;
    }

    /** @return true if both iterators are different */
    bool operator!=(const Iterator &other) {
      return this->iter_ != other.iter_;
    }

   private:
    /** Aggregates map. */
    std::unordered_map<AggregateKey, AggregateValue>::const_iterator iter_;
  };

  /** @return iterator to the start of the hash table */
  Iterator Begin() { return Iterator{ht_.cbegin()}; }

  /** @return iterator to the end of the hash table */
  Iterator End() { return Iterator{ht_.cend()}; }

 private:
  /** The hash table is just a map from aggregate keys to aggregate values.
   */
  std::unordered_map<AggregateKey, AggregateValue> ht_{};
  /** The aggregate expressions that we have. */
  const std::vector<const AbstractExpression *> &agg_exprs_;
  /** The types of aggregations that we have. */
  const std::vector<AggregationType> &agg_types_;
  // AVG={SUM,COUNT}
  std::unordered_map<AggregateKey,
                     std::unordered_map<int, std::pair<float, int>>>
      avg_map_;
};

/**
 * AggregationExecutor executes an aggregation operation (e.g. COUNT, SUM,
 MIN,
 * MAX) on the tuples of a child executor.
 */
class AggregationExecutor : public AbstractExecutor {
 public:
  /**
   * Creates a new aggregation executor.
   * @param exec_ctx the context that the aggregation should be performed in
   * @param plan the aggregation plan node
   * @param child the child executor
   */
  AggregationExecutor(ExecutorContext *exec_ctx,
                      const AggregationPlanNode *plan,
                      std::unique_ptr<AbstractExecutor> &&child);

  /** Do not use or remove this function, otherwise you will get zero points.
   */
  const AbstractExecutor *GetChildExecutor() const;

  TupleSchema *GetOutputSchema() override { return plan_->OutputSchema(); };

  void Init() override;

  RC Next(Tuple *tuple, RID *rid) override;

  /** @return the tuple as an AggregateKey */
  AggregateKey MakeKey(const Tuple *tuple) {
    std::vector<std::shared_ptr<TupleValue>> keys;
    for (const auto &expr : plan_->GetGroupBys()) {
      keys.emplace_back(expr->Evaluate(tuple, child_->GetOutputSchema()));
    }
    return {keys};
    //    return {{}};
  }

  /** @return the tuple as an AggregateValue */
  AggregateValue MakeVal(const Tuple *tuple) {
    std::vector<std::shared_ptr<TupleValue>> vals;
    for (auto &expr : plan_->GetAggregates()) {
      // auto e = dynamic_cast<const ColumnValueExpression *>(expr);
      // auto e = (ColumnValueExpression *)expr;

      // std::vector<std::shared_ptr<TupleValue>> null_group_by;
      // tuple->print(std::cout);  // debug
      // auto e = dynamic_cast<const AggregateValueExpression *>(expr);
      // std::cout << e->GetIsGroupBy() << " " << e->GetTermIdx() <<
      // std::endl; auto vals = tuple->values(); auto a = tuple->values()[0];
      // a->to_string(std::cout);
      // std::cout.flush();
      // auto v = ;
      // auto e = dynamic_cast<const ColumnValueExpression *>(expr);
      // auto f = dynamic_cast<const AggregateValueExpression *>(expr);
      vals.emplace_back(expr->Evaluate(tuple, nullptr));
    }
    return {vals};
  }

 private:
  /** The aggregation plan node. */
  const AggregationPlanNode *plan_;
  /** The child executor whose tuples we are aggregating. */
  std::unique_ptr<AbstractExecutor> child_;
  /** Simple aggregation hash table. */
  SimpleAggregationHashTable aht_;
  /** Simple aggregation hash table iterator. */
  SimpleAggregationHashTable::Iterator aht_iterator_;
};
