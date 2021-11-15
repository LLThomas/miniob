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
  Table *table = DefaultHandler::get_default().find_table(
      exec_ctx_->getDB(), plan_->GetTableName());

  RC rc;

  //单行扫描
  bool passed = false;
  while (!passed) {
    tuple->clear();
    Record record;
    record.rid = *(rid);
    RecordFileScanner scanner;
    rc = table->scan_one_tuple(&record);
    // scan record by filter
    // CompositeConditionFilter condition_filter;
    // condition_filter.init((const ConditionFilter
    // **)plan_->GetFilter().data(),
    //                       plan_->GetFilter().size());
    // rc = table->scan_one_tuple_by_filter(&record, &condition_filter);

    if (rc != RC::SUCCESS) {
      return rc;
    }

    rid->page_num = record.rid.page_num;
    rid->slot_num = record.rid.slot_num;
    //（暂时）全部行，全部列扫描
    TupleSet unused_tupleset;
    TupleRecordConverter converter{table, unused_tupleset};
    converter.record_to_tuple(tuple, &record);
    //候选操作
    passed = true;
    for (size_t i = 0; i < plan_->GetPredicates().size(); i++) {
      const std::shared_ptr<TupleValue> comp_val =
          plan_->GetPredicates()[i]->Evaluate(tuple, plan_->OutputSchema());
      if (comp_val->compare(IntValue(true)) != 0) {  // comp_val!=true
        passed = false;
        break;
      }
    }
  }
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