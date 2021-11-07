#include "sql/executor/executors/seq_scan_executor.h"

#include <iostream>

#include "sql/executor/expressions/abstract_expression.h"
#include "storage/common/condition_filter.h"
#include "storage/common/table.h"
#include "storage/default/default_handler.h"

SeqScanExecutor::SeqScanExecutor(ExecutorContext *exec_ctx,
                                 SeqScanPlanNode *plan)
    : AbstractExecutor(exec_ctx), plan_(plan) {}

void SeqScanExecutor::Init() {}

RC SeqScanExecutor::Next(Tuple *tuple, RID *rid) {
  if (rid == nullptr) {
    rid->page_num = 1;  // from 1 参考DiskBufferPool
    rid->slot_num = -1;
  }
  tuple->clear();
  RC rc;
  // //用随机数代替表
  // int random_value = rand() % 100;
  // tuple->add(random_value);
  // if (random_value % 4 != 0) {
  //   return RC::SUCCESS;
  // } else {
  //   return RC::EMPTY;
  // }

  //单行扫描
  Table *table = DefaultHandler::get_default().find_table(
      exec_ctx_->getDB(), plan_->GetTableName());
  Record record;
  record.rid = *(rid);
  RecordFileScanner scanner;
  // rc = table->scan_one_tuple(&record);
  // scan record by filter
  CompositeConditionFilter condition_filter;
  condition_filter.init((const ConditionFilter **)plan_->GetFilter().data(),
                        plan_->GetFilter().size());
  rc = table->scan_one_tuple_by_filter(&record, &condition_filter);

  LOG_ERROR("%s %s", record.data, strrc(rc));

  if (rc != RC::SUCCESS) {
    return rc;
  }

  LOG_ERROR("%s", strrc(rc));

  rid->page_num = record.rid.page_num;
  rid->slot_num = record.rid.slot_num;
  //（暂时）全部行，全部列扫描
  TupleSet unused_tupleset;
  TupleRecordConverter converter{table, unused_tupleset};
  converter.record_to_tuple(tuple, &record);
  //投影操作
  Tuple projection_tuple;
  for (auto &f : plan_->OutputSchema()->fields()) {
    projection_tuple.add(f.expr()->Evaluate(tuple, plan_->OutputSchema()));
  }
  // //输出
  tuple->operator=(std::move(projection_tuple));
  tuple->print(std::cout);  // debug
  return RC::SUCCESS;
}