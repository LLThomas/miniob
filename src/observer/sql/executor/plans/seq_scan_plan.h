#pragma once
#include "sql/executor/plans/abstract_plan.h"
#include "sql/executor/tuple.h"
class SeqScanPlanNode : public AbstractPlanNode {
 public:
  SeqScanPlanNode(TupleSchema* output, const char* table_name)
      : AbstractPlanNode(output, {}), table_name_(table_name) {}

  PlanType GetType() const override { return PlanType::SeqScan; }
  const char* GetTableName() const { return table_name_; }

 private:
  const char* table_name_;  //需要进行序列扫描的表名（或唯一标识符）
};