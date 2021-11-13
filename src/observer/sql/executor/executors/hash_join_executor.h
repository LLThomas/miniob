#ifndef __OBSERVER_SQL_HASH_JOIN_EXECUTOR_H__
#define __OBSERVER_SQL_HASH_JOIN_EXECUTOR_H__
#include "sql/executor/executor_context.h"
#include "sql/executor/executors/abstract_executor.h"
#include "sql/executor/plans/hash_join_plan.h"

/**
 * HashJoinExecutor executes a nested-loop JOIN on two tables.
 */
class HashJoinExecutor : public AbstractExecutor {
 public:
  /**
   * Construct a new HashJoinExecutor instance.
   * @param exec_ctx The executor context
   * @param plan The HashJoin join plan to be executed
   * @param left_child The child executor that produces tuples for the left side of join
   * @param right_child The child executor that produces tuples for the right side of join
   */
  HashJoinExecutor(ExecutorContext *exec_ctx, HashJoinPlanNode *plan,
                   std::unique_ptr<AbstractExecutor> &&left_child, std::unique_ptr<AbstractExecutor> &&right_child);

  /** Initialize the join */
  void Init() override;

  /**
   * Yield the next tuple from the join.
   * @param[out] tuple The next tuple produced by the join
   * @param[out] rid The next tuple RID produced by the join
   * @return `true` if a tuple was produced, `false` if there are no more tuples
   */
  RC Next(Tuple *tuple, RID *rid) override;

  /** @return The output schema for the join */
  TupleSchema *GetOutputSchema() override { return plan_->OutputSchema(); };

  Tuple* GetNextTuple() {
    if (last_tuples.size() <= 0) {
      return nullptr;
    }
    Tuple *tuple = last_tuples.front();
    last_tuples.erase(last_tuples.begin());
    return tuple;
  }

  void SetLastTuples(std::vector<Tuple *> tuples) {
    last_tuples = tuples;
  }

 private:
  /** The hash join plan node. */
  HashJoinPlanNode *plan_;
  std::unique_ptr<AbstractExecutor> left_, right_;

  std::vector<Tuple *> last_tuples;
};
#endif