

#ifndef __OBSERVER_SQL_ABSTRACT_EXECUTOR_H__
#define __OBSERVER_SQL_ABSTRACT_EXECUTOR_H__
#include "sql/executor/tuple.h"
class AbstractExecutor {
 public:
  explicit AbstractExecutor(ExecutorContext *exec_ctx) : exec_ctx_{exec_ctx} {};

  virtual ~AbstractExecutor() = default;

  virtual void Init() = 0;

  virtual RC Next(Tuple *tuple, RID *rid) = 0;
  virtual TupleSchema *GetOutputSchema() = 0;

 protected:
  ExecutorContext *exec_ctx_;
};
#endif