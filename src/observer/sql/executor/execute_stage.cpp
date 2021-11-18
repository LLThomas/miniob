/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its
affiliates. All rights reserved. miniob is licensed under Mulan PSL v2. You can
use this software according to the terms and conditions of the Mulan PSL v2. You
may obtain a copy of Mulan PSL v2 at: http://license.coscl.org.cn/MulanPSL2 THIS
SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Longda on 2021/4/13.
//

#include "execute_stage.h"

#include <sql/executor/executors/aggregation_executor.h>
#include <sql/executor/plans/aggregation_plan.h>

#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

#include "common/io/io.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/seda/timer_stage.h"
#include "event/execution_plan_event.h"
#include "event/session_event.h"
#include "event/sql_event.h"
#include "event/storage_event.h"
#include "session/session.h"
#include "sql/executor/execution_node.h"
#include "sql/executor/executor_context.h"
#include "sql/executor/executors/abstract_executor.h"
#include "sql/executor/executors/hash_join_executor.h"
#include "sql/executor/executors/seq_scan_executor.h"
#include "sql/executor/expressions/abstract_expression.h"
#include "sql/executor/expressions/aggregate_value_expression.h"
#include "sql/executor/expressions/column_value_expression.h"
#include "sql/executor/expressions/comparison_expression.h"
#include "sql/executor/expressions/constant_value_expression.h"
#include "sql/executor/plans/abstract_plan.h"
#include "sql/executor/tuple.h"
#include "storage/common/condition_filter.h"
#include "storage/common/table.h"
#include "storage/default/default_handler.h"
#include "storage/trx/trx.h"
using namespace common;

RC create_selection_executor(Trx *trx, const Selects &selects, const char *db,
                             const char *table_name,
                             SelectExeNode &select_node);

//! Constructor
ExecuteStage::ExecuteStage(const char *tag) : Stage(tag) {}

//! Destructor
ExecuteStage::~ExecuteStage() {}

//! Parse properties, instantiate a stage object
Stage *ExecuteStage::make_stage(const std::string &tag) {
  ExecuteStage *stage = new (std::nothrow) ExecuteStage(tag.c_str());
  if (stage == nullptr) {
    LOG_ERROR("new ExecuteStage failed");
    return nullptr;
  }
  stage->set_properties();
  return stage;
}

//! Set properties for this object set in stage specific properties
bool ExecuteStage::set_properties() {
  //  std::string stageNameStr(stageName);
  //  std::map<std::string, std::string> section = theGlobalProperties()->get(
  //    stageNameStr);
  //
  //  std::map<std::string, std::string>::iterator it;
  //
  //  std::string key;

  return true;
}

//! Initialize stage params and validate outputs
bool ExecuteStage::initialize() {
  LOG_TRACE("Enter");

  std::list<Stage *>::iterator stgp = next_stage_list_.begin();
  default_storage_stage_ = *(stgp++);
  mem_storage_stage_ = *(stgp++);

  LOG_TRACE("Exit");
  return true;
}

//! Cleanup after disconnection
void ExecuteStage::cleanup() {
  LOG_TRACE("Enter");

  LOG_TRACE("Exit");
}

void ExecuteStage::handle_event(StageEvent *event) {
  LOG_TRACE("Enter\n");

  handle_request(event);

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::callback_event(StageEvent *event, CallbackContext *context) {
  LOG_TRACE("Enter\n");

  // here finish read all data from disk or network, but do nothing here.
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SQLStageEvent *sql_event = exe_event->sql_event();
  sql_event->done_immediate();

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::handle_request(common::StageEvent *event) {
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SessionEvent *session_event = exe_event->sql_event()->session_event();
  Query *sql = exe_event->sqls();
  const char *current_db =
      session_event->get_client()->session->get_current_db().c_str();

  CompletionCallback *cb = new (std::nothrow) CompletionCallback(this, nullptr);
  if (cb == nullptr) {
    LOG_ERROR("Failed to new callback for ExecutionPlanEvent");
    exe_event->done_immediate();
    return;
  }
  exe_event->push_callback(cb);

  switch (sql->flag) {
    case SCF_SELECT: {  // select
      RC rc = volcano_do_select(current_db, sql,
                                exe_event->sql_event()->session_event());
      if (rc != RC::SUCCESS) {
        session_event->set_response("FAILURE\n");
      }
      exe_event->done_immediate();
    } break;

    case SCF_INSERT:
    case SCF_UPDATE:
    case SCF_DELETE:
    case SCF_CREATE_TABLE:
    case SCF_SHOW_TABLES:
    case SCF_DESC_TABLE:
    case SCF_DROP_TABLE:
    case SCF_CREATE_INDEX:
    case SCF_DROP_INDEX:
    case SCF_LOAD_DATA: {
      StorageEvent *storage_event = new (std::nothrow) StorageEvent(exe_event);
      if (storage_event == nullptr) {
        LOG_ERROR("Failed to new StorageEvent");
        event->done_immediate();
        return;
      }

      default_storage_stage_->handle_event(storage_event);
    } break;
    case SCF_SYNC: {
      RC rc = DefaultHandler::get_default().sync();
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    } break;
    case SCF_BEGIN: {
      session_event->get_client()->session->set_trx_multi_operation_mode(true);
      session_event->set_response(strrc(RC::SUCCESS));
      exe_event->done_immediate();
    } break;
    case SCF_COMMIT: {
      Trx *trx = session_event->get_client()->session->current_trx();
      RC rc = trx->commit();
      session_event->get_client()->session->set_trx_multi_operation_mode(false);
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    } break;
    case SCF_ROLLBACK: {
      Trx *trx = session_event->get_client()->session->current_trx();
      RC rc = trx->rollback();
      session_event->get_client()->session->set_trx_multi_operation_mode(false);
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    } break;
    case SCF_HELP: {
      const char *response =
          "show tables;\n"
          "desc `table name`;\n"
          "create table `table name` (`column name` `column type`, ...);\n"
          "create index `index name` on `table` (`column`);\n"
          "insert into `table` values(`value1`,`value2`);\n"
          "update `table` set column=value [where `column`=`value`];\n"
          "delete from `table` [where `column`=`value`];\n"
          "select [ * | `columns` ] from `table`;\n";
      session_event->set_response(response);
      exe_event->done_immediate();
    } break;
    case SCF_EXIT: {
      // do nothing
      const char *response = "Unsupported\n";
      session_event->set_response(response);
      exe_event->done_immediate();
    } break;
    default: {
      exe_event->done_immediate();
      LOG_ERROR("Unsupported command=%d\n", sql->flag);
    }
  }
}

void end_trx_if_need(Session *session, Trx *trx, bool all_right) {
  if (!session->is_trx_multi_operation_mode()) {
    if (all_right) {
      trx->commit();
    } else {
      trx->rollback();
    }
  }
}
//----火山模型----
struct TableInfo {
  Table *pointer;
  int table_index;
  std::vector<const AbstractExpression *> exprs;
  std::vector<std::pair<std::string, const AbstractExpression *>> on_projs;
  std::vector<std::pair<std::string, const AbstractExpression *>> select_projs;

  std::unordered_map<std::string, std::vector<const AbstractExpression *>>
      on_exprs;
  std::unordered_map<std::string, std::string> on_cols;
};
std::unique_ptr<AbstractExecutor> CreateExecutor(ExecutorContext *exec_ctx,
                                                 AbstractPlanNode *plan) {
  switch (plan->GetType()) {
    // Create a new sequential scan executor.
    case PlanType::SeqScan: {
      return std::make_unique<SeqScanExecutor>(
          exec_ctx, dynamic_cast<SeqScanPlanNode *>(plan));
    }

    // Create a new hash join executor
    case PlanType::HashJoin: {
      auto hash_join_plan = dynamic_cast<HashJoinPlanNode *>(plan);
      auto left = CreateExecutor(exec_ctx, hash_join_plan->GetLeftPlan());
      auto right = CreateExecutor(exec_ctx, hash_join_plan->GetRightPlan());
      return std::make_unique<HashJoinExecutor>(
          exec_ctx, hash_join_plan, std::move(left), std::move(right));
    }

    // Create a new aggregation executor.
    case PlanType::Aggregation: {
      auto agg_plan = dynamic_cast<const AggregationPlanNode *>(plan);
      auto child_executor = CreateExecutor(exec_ctx, agg_plan->GetChildPlan());
      return std::make_unique<AggregationExecutor>(exec_ctx, agg_plan,
                                                   std::move(child_executor));
    }

    default: {
      assert(false && "No Plan Type");
    }
  }
}
const AbstractExpression *MakeColumnValueExpression(
    const TupleSchema &schema, size_t tuple_idx, const std::string &field_name,
    std::vector<std::unique_ptr<AbstractExpression>> &expressions) {
  size_t col_idx = schema.GetFieldIdx(field_name);
  auto col_type = schema.field(col_idx).type();
  expressions.emplace_back(
      std::make_unique<ColumnValueExpression>(tuple_idx, col_idx, col_type));
  return expressions.back().get();
}

const AbstractExpression *MakeConstantValueExpression(
    const std::shared_ptr<TupleValue> &val,
    std::vector<std::unique_ptr<AbstractExpression>> &expressions) {
  expressions.emplace_back(std::make_unique<ConstantValueExpression>(val));
  return expressions.back().get();
}

const AbstractExpression *MakeComparisonExpression(
    const AbstractExpression *lhs, const AbstractExpression *rhs,
    CompOp comp_type,
    std::vector<std::unique_ptr<AbstractExpression>> &expressions) {
  expressions.emplace_back(
      std::make_unique<ComparisonExpression>(lhs, rhs, comp_type));
  return expressions.back().get();
}
const AbstractExpression *MakeAggregateValueExpression(
    bool is_group_by_term, int term_idx, AttrType col_type, FuncName agg_func,
    std::vector<std::unique_ptr<AbstractExpression>> &expressions) {
  switch (agg_func) {
    case FuncName::AGG_COUNT:
      col_type = AttrType::INTS;
      break;
    case FuncName::AGG_MIN:
    case FuncName::AGG_MAX:
      break;
    case FuncName::AGG_AVG:
      col_type = AttrType::FLOATS;
      break;
  }
  expressions.emplace_back(std::make_unique<AggregateValueExpression>(
      is_group_by_term, term_idx, col_type));
  return expressions.back().get();
}
void MakeOutputSchema(
    const std::vector<std::pair<std::string, const AbstractExpression *>>
        &exprs,
    TupleSchema *schema, const char *default_table_name) {
  for (const auto &input : exprs) {
    size_t dot = input.first.find('.');
    std::string field_name =
        dot == std::string::npos ? input.first : input.first.substr(dot + 1);
    std::string table_str = dot == std::string::npos
                                ? default_table_name
                                : input.first.substr(0, dot);
    schema->add(TupleField{input.second->GetReturnType(), table_str.c_str(),
                           field_name.c_str(), input.second});
  }
}

const AbstractExpression *ParseConditionToExpression(
    const RelAttr rel, const Value val, const int is_attr, const bool is_right,
    std::unordered_map<std::string, TableInfo> &table_infos,
    std::vector<std::unique_ptr<AbstractExpression>> &expressions) {
  if (is_attr == 1) {
    std::string table_name;
    if (rel.relation_name == nullptr) {
      table_name = table_infos[""].pointer->name();
    } else {
      table_name = rel.relation_name;
    }
    TupleSchema schema;
    TupleSchema::from_table(table_infos[table_name].pointer, schema);
    if (is_right) {
      return MakeColumnValueExpression(
          schema, 1, table_name + "." + rel.attribute_name, expressions);
    }
    return MakeColumnValueExpression(
        schema, 0, table_name + "." + rel.attribute_name, expressions);
  } else {
    switch (val.type) {
      case AttrType::INTS: {
        return MakeConstantValueExpression(
            std::make_shared<IntValue>(*((int *)val.data)), expressions);
      }
      case AttrType::FLOATS: {
        return MakeConstantValueExpression(
            std::make_shared<FloatValue>(*((float *)val.data)), expressions);
      }
      case AttrType::DATES: {
        uint16_t d;
        RC rc = serialize_date(&d, (const char *)val.data);
        if (rc != SUCCESS) {
          LOG_ERROR("Invalid date: %s", val.data);
          return nullptr;
        }
        return MakeConstantValueExpression(std::make_shared<DateValue>(d),
                                           expressions);
      }
      case AttrType::CHARS: {
        return MakeConstantValueExpression(
            std::make_shared<StringValue>((const char *)val.data), expressions);
      }
      case AttrType::NULLS: {
        return MakeConstantValueExpression(std::make_shared<NullValue>(),
                                           expressions);
      }
      default:
        // TODO: 报错，非数值类型
        return nullptr;
    }
  }
}

RC PlanSelect(const char *db, Selects &selects, std::vector<Table *> tables,
              std::vector<std::unique_ptr<AbstractExpression>> &out_exprs,
              std::vector<std::string> &out_projections,
              std::unordered_map<std::string, TableInfo> &table_infos) {
  //特殊情况：只有一个*
  if (selects.attr_num > 0 &&
      std::string(selects.attributes[0].attribute_name) == "*" &&
      selects.attributes[0].relation_name == nullptr) {
    for (size_t i = 0; i < tables.size(); i++) {
      selects.attributes[tables.size() - i - 1].relation_name =
          (char *)tables[i]->name();
      selects.attributes[tables.size() - i - 1].attribute_name =
          (char *)(new std::string("*"))->c_str();
    }
    selects.attr_num = tables.size();
  }
  for (int i = selects.attr_num - 1; i >= 0; i--) {
    RelAttr ra = selects.attributes[i];
    std::string table_name =
        ra.relation_name == nullptr ? tables[0]->name() : ra.relation_name;
    std::string attr_name = ra.attribute_name;
    Table *current_table =
        DefaultHandler::get_default().find_table(db, table_name.c_str());
    {  //【元数据校验】
       // select table check
      if (current_table == nullptr) {
        LOG_WARN("No such table [%s] in db [%s]", table_name, db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      // select attr check
      if (attr_name != "*" &&
          current_table->table_meta().field(attr_name.c_str()) == nullptr) {
        LOG_WARN("No such field. %s.%s", table_name, attr_name);
        return RC::SCHEMA_FIELD_MISSING;
      }
    }
    TupleSchema current_schema;
    TupleSchema::from_table(current_table, current_schema);
    if (attr_name == "*") {
      // "*" 代表映射所有字段
      for (size_t f = 0; f < current_schema.fields().size(); f++) {
        const char *temp_attr_name = current_schema.field(f).field_name();
        std::string field_name = table_name + "." + temp_attr_name;
        out_projections.push_back(field_name);
        table_infos[table_name].select_projs.push_back(
            {field_name, MakeColumnValueExpression(current_schema, 0,
                                                   field_name, out_exprs)});
      }
    } else {
      std::string field_name = table_name + "." + attr_name;
      out_projections.push_back(field_name);
      table_infos[table_name].select_projs.push_back(
          {field_name, MakeColumnValueExpression(current_schema, 0, field_name,
                                                 out_exprs)});
    }
  }
  return RC::SUCCESS;
}
RC PlanFrom(const char *db, const Selects &selects,
            std::vector<Table *> &out_tables,
            std::unordered_map<std::string, TableInfo> &table_infos) {
  for (int i = selects.relation_num - 1; i >= 0; i--) {
    Table *table =
        DefaultHandler::get_default().find_table(db, selects.relations[i]);
    // from table check
    if (table == nullptr) {
      LOG_WARN("No such table [%s] in db [%s]", selects.relations[i], db);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    out_tables.emplace_back(table);
  }
  // 2. sort from_tables by table length
  // std::sort(out_tables.begin(), out_tables.end(),
  //           [](const Table *t1, const Table *t2) {
  //             return t1->table_meta().record_size() <
  //                    t2->table_meta().record_size();
  //           });
  // construct
  table_infos[""] = TableInfo();
  table_infos[""].pointer = out_tables[0];
  for (size_t i = 0; i < out_tables.size(); i++) {
    std::string info_key = out_tables[i]->name();
    table_infos[info_key] = TableInfo();
    table_infos[info_key].table_index = i;
    table_infos[info_key].pointer = out_tables[i];
  }
  return RC::SUCCESS;
}
RC PlanWhere(const char *db, const Selects &selects,
             std::vector<std::unique_ptr<AbstractExpression>> &out_exprs,
             std::vector<const AbstractExpression *> &out_predicates,
             std::unordered_map<std::string, TableInfo> &table_infos) {
  for (size_t i = 0; i < selects.condition_num; i++) {
    Condition con = selects.conditions[i];
    std::string left_attr_table_name = "";
    std::string right_attr_table_name = "";
    bool is_join = false;
    if (con.left_is_attr && con.right_is_attr) {
      is_join = true;
    }
    // 对 DATE 类型进行特殊处理
    if (con.left_is_attr) {
      left_attr_table_name = con.left_attr.relation_name == nullptr
                                 ? ""
                                 : con.left_attr.relation_name;
      // condition table check
      if (left_attr_table_name != "" &&
          DefaultHandler::get_default().find_table(
              db, left_attr_table_name.c_str()) == nullptr) {
        LOG_WARN("No such table [%s] in db [%s]", left_attr_table_name, db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      // condition attr check
      if (table_infos[left_attr_table_name].pointer->table_meta().field(
              con.left_attr.attribute_name) == nullptr) {
        LOG_WARN("No such field. %s.%s", left_attr_table_name,
                 left_attr_table_name);
        return RC::SCHEMA_FIELD_MISSING;
      }
      AttrType left_type = table_infos[left_attr_table_name]
                               .pointer->table_meta()
                               .field(con.left_attr.attribute_name)
                               ->type();

      if (left_type == DATES && !con.right_is_attr &&
          con.right_value.type == CHARS)
        con.right_value.type = DATES;
    }
    if (con.right_is_attr) {
      right_attr_table_name = con.right_attr.relation_name == nullptr
                                  ? ""
                                  : con.right_attr.relation_name;
      // condition table check
      if (right_attr_table_name != "" &&
          DefaultHandler::get_default().find_table(
              db, right_attr_table_name.c_str()) == nullptr) {
        LOG_WARN("No such table [%s] in db [%s]", right_attr_table_name, db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      // condition attr check
      if (table_infos[right_attr_table_name].pointer->table_meta().field(
              con.right_attr.attribute_name) == nullptr) {
        LOG_WARN("No such field. %s.%s", right_attr_table_name,
                 right_attr_table_name);
        return RC::SCHEMA_FIELD_MISSING;
      }
      AttrType right_type = table_infos[right_attr_table_name]
                                .pointer->table_meta()
                                .field(con.right_attr.attribute_name)
                                ->type();
      if (right_type == DATES && !con.left_is_attr &&
          con.left_value.type == CHARS)
        con.left_value.type = DATES;
    }

    const AbstractExpression *lhs = nullptr;
    const AbstractExpression *rhs = nullptr;
    if (is_join) {
      if (table_infos[left_attr_table_name].table_index <
          table_infos[right_attr_table_name].table_index) {
        lhs = ParseConditionToExpression(con.left_attr, con.left_value,
                                         con.left_is_attr, false, table_infos,
                                         out_exprs);
        rhs = ParseConditionToExpression(con.right_attr, con.right_value,
                                         con.right_is_attr, true, table_infos,
                                         out_exprs);

      } else {
        rhs = ParseConditionToExpression(con.left_attr, con.left_value,
                                         con.left_is_attr, true, table_infos,
                                         out_exprs);
        lhs = ParseConditionToExpression(con.right_attr, con.right_value,
                                         con.right_is_attr, false, table_infos,
                                         out_exprs);
      }

      if (con.comp == CompOp::EQUAL_TO) {
        table_infos[left_attr_table_name].on_cols[right_attr_table_name] =
            left_attr_table_name + "." + con.left_attr.attribute_name;
        table_infos[right_attr_table_name].on_cols[left_attr_table_name] =
            right_attr_table_name + "." + con.right_attr.attribute_name;
      } else {
        const AbstractExpression *on_condition =
            MakeComparisonExpression(lhs, rhs, con.comp, out_exprs);

        table_infos[right_attr_table_name]
            .on_exprs[left_attr_table_name]
            .push_back(on_condition);
        table_infos[left_attr_table_name]
            .on_exprs[right_attr_table_name]
            .push_back(on_condition);
      }

    } else {
      lhs = ParseConditionToExpression(con.left_attr, con.left_value,
                                       con.left_is_attr, false, table_infos,
                                       out_exprs);
      if (con.comp != IS_LEFT_NULL && con.comp != IS_LEFT_NOT_NULL) {
        //        if (is_join) {
        //          rhs = ParseConditionToExpression(con.right_attr,
        //          con.right_value,
        //                                           con.right_is_attr, true,
        //                                           table_infos, out_exprs);
        //        } else {
        rhs = ParseConditionToExpression(con.right_attr, con.right_value,
                                         con.right_is_attr, false, table_infos,
                                         out_exprs);
        //        }
      }
    }

    if (lhs == nullptr || (con.comp != IS_LEFT_NULL &&
                           con.comp != IS_LEFT_NOT_NULL && rhs == nullptr)) {
      return RC::INVALID_ARGUMENT;
    }

    if (is_join) {
      table_infos[left_attr_table_name].on_projs.push_back(
          {left_attr_table_name + "." + con.left_attr.attribute_name, lhs});
      table_infos[right_attr_table_name].on_projs.push_back(
          {right_attr_table_name + "." + con.right_attr.attribute_name, rhs});
    } else {
      const AbstractExpression *comp_exp =
          MakeComparisonExpression(lhs, rhs, con.comp, out_exprs);
      out_predicates.emplace_back(comp_exp);
      if (con.left_is_attr) {
        table_infos[left_attr_table_name].exprs.emplace_back(comp_exp);
      }
      if (con.right_is_attr) {
        table_infos[right_attr_table_name].exprs.emplace_back(comp_exp);
      }
    }
  }

  return RC::SUCCESS;
}
SeqScanPlanNode *ConstructScanTable(
    const char *scan_table_name,
    std::unordered_map<std::string, TableInfo> &table_infos) {
  TupleSchema *scan_schema = new TupleSchema;
  //    std::unique_ptr<SeqScanPlanNode> scan_plan_1;
  std::vector<std::pair<std::string, const AbstractExpression *>> scan_proj;
  // from select
  if (table_infos[scan_table_name].select_projs.empty() == false) {
    scan_proj.insert(scan_proj.end(),
                     table_infos[scan_table_name].select_projs.begin(),
                     table_infos[scan_table_name].select_projs.end());
  }
  // from on
  if (table_infos[scan_table_name].on_projs.empty() == false) {
    //去重
    for (auto &e : table_infos[scan_table_name].on_projs) {
      bool is_unique = true;
      for (auto &o : scan_proj) {
        if (o.first == e.first) {
          is_unique = false;
          break;
        }
      }
      if (is_unique) scan_proj.push_back(e);
    }
  }

  MakeOutputSchema(scan_proj, scan_schema, scan_table_name);
  //    scan_plan_1 = std::make_unique<SeqScanPlanNode>(
  //        scan_schema_1, scan_table_1_name,
  //        table_to_exp[scan_table_1_name]
  //    );
  return new SeqScanPlanNode(scan_schema, scan_table_name,
                             table_infos[scan_table_name].exprs);
}

RC PlanAggregation(const Selects &selects, AbstractPlanNode *table_plan,
                   const char *table_name, const TupleSchema &table_schema,
                   std::unordered_map<std::string, TableInfo> &table_infos,
                   std::vector<AbstractPlanNode *> &out_plans) {
  // table info
  TableInfo scan_table_info = table_infos[table_name];
  // enum -> name
  std::unordered_map<int, std::string> agg_to_name;
  agg_to_name[FuncName::AGG_MAX] = "max";
  agg_to_name[FuncName::AGG_MIN] = "min";
  agg_to_name[FuncName::AGG_COUNT] = "count";
  agg_to_name[FuncName::AGG_AVG] = "avg";
  // 【元数据校验】
  for (int i = 0; i < selects.aggregation_num; i++) {
    Aggregation agg = selects.aggregations[i];
    std::string attr_name(agg.attribute.attribute_name);
    {
      // select attr check
      if (attr_name != "*" && scan_table_info.pointer->table_meta().field(
                                  attr_name.c_str()) == nullptr) {
        return RC::FORMAT;
      }
    }
  }
  // 1.agg plan node and combine
  TupleSchema *agg_schema = new TupleSchema();
  // col exp, agg exp -> schema
  auto col_exp = new std::vector<std::unique_ptr<AbstractExpression>>{};
  std::vector<const AbstractExpression *> col_exps;
  auto agg_exp = new std::vector<std::unique_ptr<AbstractExpression>>{};
  std::vector<std::pair<std::string, const AbstractExpression *>> agg_exps;
  std::vector<AggregationType> agg_types;
  for (int i = 0; i < selects.aggregation_num; i++) {
    Aggregation agg = selects.aggregations[i];
    std::string agg_str =
        agg_to_name[agg.func_name] + "(" + agg.attribute.attribute_name + ")";
    // bool is_star_attr = ;
    // if (agg.func_name != FuncName::AGG_COUNT && is_star_attr) {
    //   return RC::INVALID_ARGUMENT;
    // }
    std::string attr_name(agg.attribute.attribute_name);
    if (attr_name == "*") {
      if (agg.func_name != FuncName::AGG_COUNT) {
        return RC::FORMAT;
      }
      attr_name = table_schema.field(0).field_name();
    }

    col_exps.push_back(MakeColumnValueExpression(
        table_schema, 0, std::string(table_name) + "." + attr_name, *col_exp));
    agg_exps.push_back({agg_str, MakeAggregateValueExpression(
                                     false, table_schema.GetColIdx(attr_name),
                                     col_exp->back()->GetReturnType(),
                                     agg.func_name, *agg_exp)});
    switch (agg.func_name) {
      case FuncName::AGG_MAX:
        agg_types.push_back(AggregationType::MaxAggregate);
        break;
      case FuncName::AGG_MIN:
        agg_types.push_back(AggregationType::MinAggregate);
        break;
      case FuncName::AGG_AVG:
        agg_types.push_back(AggregationType::AvgAggregate);
        break;
      case FuncName::AGG_COUNT:
        agg_types.push_back(AggregationType::CountAggregate);
        break;
    }
  }
  MakeOutputSchema(agg_exps, agg_schema, table_name);
  // agg plan
  AggregationPlanNode *agg_plan =
      new AggregationPlanNode(agg_schema, table_plan, nullptr, {},
                              std::move(col_exps), std::move(agg_types));

  out_plans.emplace_back(agg_plan);
  return RC::SUCCESS;
}
RC BuildQueryPlan(std::vector<AbstractPlanNode *> &out_plans,
                  std::vector<std::unique_ptr<AbstractExpression>> &out_exprs,
                  const char *db, const Selects &selects) {
  RC rc = RC::SUCCESS;
  // ---------------Sec Scan Plan---------------

  // 1. parse FROM
  std::vector<Table *> from_tables;  //表指针的数组
  std::unordered_map<std::string, TableInfo>
      table_infos;  // 生成哈希表{表名,表信息}，必有{"",第一个表}
  rc = PlanFrom(db, selects, from_tables, table_infos);
  if (rc != RC::SUCCESS) return rc;
  // 2. parse WHERE
  std::vector<const AbstractExpression *> predicates;  //默认全是用AND连接的
  rc = PlanWhere(db, selects, out_exprs, predicates, table_infos);
  if (rc != RC::SUCCESS) return rc;

  // 3. parse SELECT
  std::vector<std::string> projections_str;
  rc = PlanSelect(db, (Selects &)selects, from_tables, out_exprs,
                  projections_str, table_infos);
  if (rc != RC::SUCCESS) return rc;
  //-----------------------------------
  const char *last_scan_table_name = from_tables[0]->name();
  AbstractPlanNode *last_plan;
  if (from_tables.size() == 1) {
    TupleSchema *table_schema = new TupleSchema();
    TupleSchema::from_table(from_tables[0], *table_schema);
    last_plan =
        new SeqScanPlanNode(table_schema, last_scan_table_name, predicates);
  } else
    last_plan = ConstructScanTable(last_scan_table_name, table_infos);
  std::vector<std::string> last_scan_table_names{last_scan_table_name};
  // construct outher scan table and join them
  for (int i = 1; i < (int)from_tables.size(); i++) {
    // construct next scan table
    const char *current_scan_table_name = from_tables[i]->name();
    SeqScanPlanNode *current_plan =
        ConstructScanTable(current_scan_table_name, table_infos);

    // join
    // 拼接左右SecScan的OutputSchema
    TupleSchema *join_schema = new TupleSchema;
    join_schema->append(*last_plan->OutputSchema());
    join_schema->append(*current_plan->OutputSchema());
    join_schema->print(std::cout, true);
    join_schema->printExprs(std::cout);
    //要从以前所有的name里找，可以构建出来的name
    HashJoinPlanNode *join_plan = nullptr;
    for (auto &str : last_scan_table_names) {
      if (table_infos[str].on_cols.count(current_scan_table_name) > 0 &&
          table_infos[current_scan_table_name].on_cols.count(str) > 0) {
        join_plan = new HashJoinPlanNode(
            join_schema,
            std::vector<AbstractPlanNode *>{last_plan, current_plan}, {},
            table_infos[str].on_cols[current_scan_table_name],
            table_infos[current_scan_table_name].on_cols[str]);
        break;
      }
    }
    //全连接
    if (join_plan == nullptr) {
      //添加exprs
      std::vector<const AbstractExpression *> predicates;
      for (auto &str : last_scan_table_names) {
        if (table_infos[str].on_exprs.count(current_scan_table_name) > 0) {
          predicates.insert(
              predicates.end(),
              table_infos[str].on_exprs[current_scan_table_name].begin(),
              table_infos[str].on_exprs[current_scan_table_name].end());
        }
      }
      join_plan = new HashJoinPlanNode(
          join_schema, std::vector<AbstractPlanNode *>{last_plan, current_plan},
          predicates, "", "");
    }

    // update scan_table_1
    last_scan_table_names.emplace_back(current_scan_table_name);
    //      left_plan = std::move(join_plan);
    last_plan = join_plan;
  }
  //    out_plans.emplace_back(std::move(left_plan));
  out_plans.emplace_back(last_plan);
  TupleSchema *old_schema = out_plans[0]->OutputSchema();
  if (selects.aggregation_num > 0)
    return PlanAggregation(selects, last_plan, from_tables[0]->name(),
                           *old_schema, table_infos, out_plans);
  //最后修改root plan的投影
  TupleSchema *out_schema = new TupleSchema;
  std::vector<std::pair<std::string, const AbstractExpression *>> projections;
  for (auto s : projections_str) {
    projections.push_back(
        {s, MakeColumnValueExpression(*old_schema, 0, s, out_exprs)});
  }
  MakeOutputSchema(projections, out_schema, from_tables[0]->name());
  out_plans[0]->SetOutputSchema(out_schema);

  return RC::SUCCESS;
}

RC ExecuteStage::volcano_do_select(const char *db, const Query *sql,
                                   SessionEvent *session_event) {
  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  std::stringstream ss;

  // build query plan
  //  std::vector<std::unique_ptr<AbstractPlanNode>>
  //      allocated_plans;  //储存计划树的所有节点（指针均指向堆）
  std::vector<AbstractPlanNode *> allocated_plans;  //储存所有表达式
  std::vector<std::unique_ptr<AbstractExpression>>
      allocated_expressions;  //储存所有表达式
  std::vector<std::unique_ptr<TupleSchema>> allocated_schemas;  //储存所有schema
  rc = BuildQueryPlan(allocated_plans, allocated_expressions, db,
                      sql->sstr.selection);
  if (rc != RC::SUCCESS) {
    end_trx_if_need(session, trx, false);
    return rc;
  }
  allocated_plans[0]->OutputSchema()->print(
      ss, sql->sstr.selection.relation_num > 1);
  // execute plan
  ExecutorContext exec_ctx{trx, db};
  //  auto executor = CreateExecutor(&exec_ctx, allocated_plans[0].get());
  auto executor = CreateExecutor(&exec_ctx, allocated_plans[0]);

  std::cout << "executor: "
            << executor->GetOutputSchema()->fields()[0].field_name()
            << std::endl;

  executor->Init();

  Tuple tuple;
  RID rid;
  rid.page_num = 1;
  rid.slot_num = -1;
  while ((rc = executor->Next(&tuple, &rid)) == RC::SUCCESS) {
    tuple.print(ss);
  }

  if (rc != RC::RECORD_EOF) {
    end_trx_if_need(session, trx, false);
    return rc;
  }
  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);

  return RC::SUCCESS;
}