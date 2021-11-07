#ifndef __OBSERVER_SQL_SEQ_SCAN_PLAN_H__
#define __OBSERVER_SQL_SEQ_SCAN_PLAN_H__
#include "sql/executor/plans/abstract_plan.h"
#include "sql/executor/tuple.h"
#include "storage/common/condition_filter.h"

class SeqScanPlanNode : public AbstractPlanNode {
 public:
  SeqScanPlanNode(TupleSchema* output, const char* table_name, std::vector<DefaultConditionFilter *> condition_filters_)
      : AbstractPlanNode(output, {}), table_name_(table_name), condition_filters_(condition_filters_) {}

  PlanType GetType() const override { return PlanType::SeqScan; }
  const char* GetTableName() const { return table_name_; }
  std::vector<DefaultConditionFilter *> GetFilter() {
    return condition_filters_;
  }

 private:
  const char* table_name_;  //需要进行序列扫描的表名（或唯一标识符）
  std::vector<DefaultConditionFilter *> condition_filters_;
};
#endif