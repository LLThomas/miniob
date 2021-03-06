#ifndef __OBSERVER_SQL_EXECUTOR_CONTEXT_H__
#define __OBSERVER_SQL_EXECUTOR_CONTEXT_H__
#include <unordered_set>
#include <utility>
#include <vector>

#include "storage/trx/trx.h"
class ExecutorContext {
 public:
  ExecutorContext(Trx *trx, const char *db) : trx_(trx), db_(db) {}

  ~ExecutorContext() = default;

  const char *getDB() const { return db_; }
  Trx *getTransaction() const { return trx_; }

 private:
  Trx *trx_;
  const char *db_;
};
#endif