#include "sql/executor/executors/seq_scan_executor.h"

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
  rc = table->scan_one_tuple(&record);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  rid->page_num = record.rid.page_num;
  rid->slot_num = record.rid.slot_num;
  TupleSet unused_tupleset;  //暂时全部扫描
  TupleRecordConverter converter{table, unused_tupleset};
  converter.record_to_tuple(tuple, &record);

  return RC::SUCCESS;
}