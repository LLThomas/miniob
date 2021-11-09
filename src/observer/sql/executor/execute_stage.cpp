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
std::string agg_to_string(Aggregation agg) {
  std::string res = "";
  // funcname
  switch (agg.func_name) {
    case FuncName::AGG_MAX:
      res += "max";
      break;
    case FuncName::AGG_MIN:
      res += "min";
      break;
    case FuncName::AGG_COUNT:
      res += "count";
      break;
    case FuncName::AGG_AVG:
      res += "avg";
      break;
  }
  res += "(";
  // expression
  if (agg.is_value) {
    AttrType type = agg.value.type;
    void *val = agg.value.data;
    std::string str;
    switch (type) {
      case AttrType::INTS: {
        str = std::to_string(*((int *)val));
        break;
      }
      case AttrType::FLOATS: {
        FloatValue fv(*((float *)val));
        std::stringstream ss;
        fv.to_string(ss);
        ss >> str;
        break;
      }
      case AttrType::DATES: {
        DateValue dv(*((uint16_t *)val));
        std::stringstream ss;
        dv.to_string(ss);
        ss >> str;
        break;
      }
      default:
        // TODO: 报错，非数值类型
        break;
    }
    res += str;
  } else {
    //如果有relation_name就要补上
    if (agg.attribute.relation_name != nullptr) {
      res += agg.attribute.relation_name;
      res += ".";
    }
    // field_name
    res += agg.attribute.attribute_name;
  }
  res += ")";
  return res;
}
//聚合函数
void aggregation_exec(const Selects &selects, TupleSet *res_tuples) {
  if (selects.aggregation_num > 0) {
    //目前agg只涉及单表情况
    const char *table_name = selects.relations[0];
    //先设置schema
    TupleSchema agg_schema;
    for (size_t i = 0; i < selects.aggregation_num; i++) {
      const Aggregation &agg = selects.aggregations[i];
      //设置type
      AttrType attr = AttrType::UNDEFINED;
      switch (agg.func_name) {
        case FuncName::AGG_MAX:
        case FuncName::AGG_MIN: {
          if (agg.is_value) {
            attr = agg.value.type;
          } else {
            //获取对应的field
            const TupleSchema &ts = res_tuples->get_schema();
            const TupleField &tf = ts.field(ts.index_of_field(
                table_name,
                agg.attribute.attribute_name));  //获取func(age)的age
            attr = tf.type();
          }

          break;
        }

        case FuncName::AGG_COUNT:
          attr = AttrType::INTS;
          break;
        case FuncName::AGG_AVG:
          attr = AttrType::FLOATS;
          break;
      }

      agg_schema.add(attr, table_name, agg_to_string(agg).c_str());
    }
    //再依次添加字段值
    Tuple out;
    for (size_t i = 0; i < selects.aggregation_num; i++) {
      const Aggregation &agg = selects.aggregations[i];
      const std::vector<Tuple> &tuples = res_tuples->tuples();
      switch (agg.func_name) {
        case FuncName::AGG_MAX:
        case FuncName::AGG_MIN: {
          int type = agg.func_name == FuncName::AGG_MAX ? 1 : -1;

          //遍历所有元组，获取最大值
          int target_idx = 0;
          for (size_t t = 1; t < tuples.size(); t++) {
            if (type * tuples[t]
                           .get(res_tuples->get_schema().index_of_field(
                               table_name, agg.attribute.attribute_name))
                           .compare(tuples[target_idx].get(
                               res_tuples->get_schema().index_of_field(
                                   table_name, agg.attribute.attribute_name))) >
                0) {
              target_idx = t;
            }
          }
          //增加这条记录
          out.add(tuples[target_idx].get_pointer(
              res_tuples->get_schema().index_of_field(
                  table_name, agg.attribute.attribute_name)));

          break;
        }
        case FuncName::AGG_COUNT: {
          // 值为size的大小
          //增加这条记录
          out.add((int)tuples.size());
          break;
        }
        case FuncName::AGG_AVG: {
          //遍历所有元组，获取和
          float sum = 0;
          for (size_t t = 0; t < tuples.size(); t++) {
            AttrType type = tuples[t]
                                .get(res_tuples->get_schema().index_of_field(
                                    table_name, agg.attribute.attribute_name))
                                .get_type();
            switch (type) {
              case AttrType::INTS:
                sum += ((IntValue &)tuples[t].get(
                            res_tuples->get_schema().index_of_field(
                                table_name, agg.attribute.attribute_name)))
                           .get_value();
                break;
              case AttrType::FLOATS:
                sum += ((FloatValue &)tuples[t].get(
                            res_tuples->get_schema().index_of_field(
                                table_name, agg.attribute.attribute_name)))
                           .get_value();
                break;
              case AttrType::DATES:
                sum += ((DateValue &)tuples[t].get(
                            res_tuples->get_schema().index_of_field(
                                table_name, agg.attribute.attribute_name)))
                           .get_value();
                break;
              default:
                // TODO: 报错，非数值类型
                break;
            }
          }
          //增加这条记录
          out.add(sum / tuples.size());
          break;
        }
      }
    }
    //等所有值都计算完再去清除res
    res_tuples->clear();
    res_tuples->set_schema(agg_schema);
    res_tuples->add(std::move(out));
  }

  return;
}

//需要满足多表联查条件
bool match_join_condition(const Tuple *res_tuple,
                          const std::vector<std::vector<int>> condition_idxs) {
  for (size_t i = 0; i < condition_idxs.size(); ++i) {
    std::shared_ptr<TupleValue> left_val =
        res_tuple->get_pointer(condition_idxs[i][0]);
    int op = condition_idxs[i][1];
    std::shared_ptr<TupleValue> right_val =
        res_tuple->get_pointer(condition_idxs[i][2]);
    switch (op) {
      case CompOp::EQUAL_TO:
        if (left_val.get()->compare(*right_val.get()) != 0) {
          return false;
        }
        break;
      default:
        break;
    }
  }
  return true;
}
//将多段小元组合成一个大元组
Tuple merge_tuples(
    const std::vector<std::vector<Tuple>::const_iterator> temp_tuples,
    std::vector<int> orders) {
  std::vector<std::shared_ptr<TupleValue>> temp_res;
  Tuple res_tuple;
  for (size_t t = 0; t < temp_tuples.size(); t++) {
    for (int idx = 0; idx < (*temp_tuples[t]).size(); idx++) {
      //先把每个字段都放到各应的位置上
      temp_res.push_back((*temp_tuples[t]).get_pointer(idx));
    }
  }
  for (size_t i = 0; i < temp_res.size(); i++) {
    //再依次添加到大元组里即可
    res_tuple.add(temp_res[orders[i]]);
  }
  return res_tuple;
}
// 这里没有对输入的某些信息做合法性校验，比如查询的列名、where条件中的列名等，没有做必要的合法性校验
// 需要补充上这一部分.
// 校验部分也可以放在resolve，不过跟execution放一起也没有关系
RC ExecuteStage::do_select(const char *db, const Query *sql,
                           SessionEvent *session_event) {
  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  const Selects &selects = sql->sstr.selection;

  // 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select
  // 执行节点
  std::vector<SelectExeNode *> select_nodes;
  for (size_t i = 0; i < selects.relation_num; i++) {
    const char *table_name = selects.relations[i];
    SelectExeNode *select_node = new SelectExeNode;
    rc = create_selection_executor(trx, selects, db, table_name, *select_node);
    if (rc != RC::SUCCESS) {
      delete select_node;
      for (SelectExeNode *&tmp_node : select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    }
    select_nodes.push_back(select_node);
  }
  if (select_nodes.empty()) {
    LOG_ERROR("No table given");
    end_trx_if_need(session, trx, false);
    return RC::SQL_SYNTAX;
  }

  std::vector<TupleSet> tuple_sets;
  for (SelectExeNode *&node : select_nodes) {
    TupleSet tuple_set;
    rc = node->execute(tuple_set);
    if (rc != RC::SUCCESS) {
      for (SelectExeNode *&tmp_node : select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    } else {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }

  std::stringstream ss;
  TupleSet print_tuples;
  if (tuple_sets.size() > 1) {
    // 本次查询了多张表，需要做join操作
    // 【每个输出元组的列名需要扩展为[tableName].[colName]的形式】
    // 测试环境会保证多表查询的每个attr都是*或table.id的格式，不会出现单单的id
    // 需要按照selects.attribute的顺序添加，如果是t1.*就添加整张表
    TupleSchema join_schema;
    //仅仅是把每个表的每个字段无序拼接起来，用于下面的对应和查找
    TupleSchema old_schema;
    for (std::vector<TupleSet>::const_reverse_iterator
             rit = tuple_sets.rbegin(),
             rend = tuple_sets.rend();
         rit != rend; ++rit) {
      //这里是某张表投影完的所有字段，如果是select * from t1,t2;
      // old_schema=[t1.a, t1.b, t2.a, t2.b]
      old_schema.append(rit->get_schema());
    }

    //对照着列名的输出顺序。之后输出一行tuple的时候，tuple.col[i]就输出到select_order[i]的位置
    std::vector<int> select_order;
    for (int i = selects.attr_num - 1; i >= 0; i--) {
      const RelAttr &rel_attr = selects.attributes[i];
      if (0 == strcmp("*", rel_attr.attribute_name)) {
        if (rel_attr.relation_name == nullptr) {
          //如果是select * ，添加所有字段
          for (size_t f = 0; f < old_schema.fields().size(); f++) {
            join_schema.add(old_schema.fields()[f]);
            select_order.push_back(f);
          }
        } else {
          //如果是select t1.*，表名匹配的加入字段
          for (size_t f = 0; f < old_schema.fields().size(); f++) {
            if (0 == strcmp(old_schema.fields()[f].table_name(),
                            rel_attr.relation_name)) {
              join_schema.add(old_schema.fields()[f]);
              select_order.push_back(f);
            }
          }
        }
      } else {
        //如果是select t1.age，表名+字段名匹配的加入字段
        int f = old_schema.index_of_field(rel_attr.relation_name,
                                          rel_attr.attribute_name);
        join_schema.add(old_schema.fields()[f]);
        select_order.push_back(f);
      }
    }

    print_tuples.set_schema(join_schema);
    // 【联查的conditions需要找到对应的表】
    // C x 3数组
    // 每一条的3个元素代表（左值的属性在新schema的下标，CompOp运算符，右值的属性在新schema的下标）
    std::vector<std::vector<int>> condition_idxs;
    for (size_t i = 0; i < selects.condition_num; i++) {
      const Condition &condition = selects.conditions[i];
      if (condition.left_is_attr == 1 &&
          condition.right_is_attr == 1)  // 左右都是属性名，并且表名都符合
      {
        std::vector<int> temp_con;
        const char *l_table_name = condition.left_attr.relation_name;
        const char *l_field_name = condition.left_attr.attribute_name;
        const CompOp comp = condition.comp;
        const char *r_table_name = condition.right_attr.relation_name;
        const char *r_field_name = condition.right_attr.attribute_name;
        temp_con.push_back(print_tuples.get_schema().index_of_field(
            l_table_name, l_field_name));
        temp_con.push_back(comp);
        temp_con.push_back(print_tuples.get_schema().index_of_field(
            r_table_name, r_field_name));
        condition_idxs.push_back(temp_con);
      }
    }
    // 【元组的拼接需要实现笛卡尔积】
    // 给每个表分配一个pos指针，另外还需要一个输出用的temp_tuple
    std::vector<std::vector<Tuple>::const_iterator> tuple_poses;
    std::vector<std::vector<Tuple>::const_iterator> tuple_poses_begin;
    std::vector<std::vector<Tuple>::const_iterator> tuple_poses_end;

    std::vector<std::vector<Tuple>::const_iterator> temp_tuples;
    for (std::vector<TupleSet>::const_reverse_iterator
             rit = tuple_sets.rbegin(),
             rend = tuple_sets.rend();
         rit != rend; ++rit) {
      tuple_poses.push_back(rit->tuples().begin());
      tuple_poses_begin.push_back(rit->tuples().begin());
      tuple_poses_end.push_back(rit->tuples().end());
      temp_tuples.push_back(rit->tuples().begin());
    }
    const size_t N = tuple_sets.size();
    //每个表都有数据才会有结果集，一旦有空表就是空结果
    if (temp_tuples.size() == N) {
      //补满后就输出
      Tuple res_tuple = merge_tuples(temp_tuples, select_order);
      if (match_join_condition(&res_tuple, condition_idxs))
        print_tuples.add(std::move(res_tuple));
      while (tuple_poses[0] != tuple_poses_end[0]) {
        //弹出最末的Tuple
        temp_tuples.pop_back();
        //前进一步指针，如果越界，就回到表头(除了结束)，并继续弹出Tuple
        tuple_poses[temp_tuples.size()]++;
        if (tuple_poses[temp_tuples.size()] ==
            tuple_poses_end[temp_tuples.size()]) {
          if (temp_tuples.size() > 0)
            tuple_poses[temp_tuples.size()] =
                tuple_poses_begin[temp_tuples.size()];
          continue;
        }
        //将元组补满
        while (temp_tuples.size() < N) {
          temp_tuples.push_back(tuple_poses[temp_tuples.size()]);
        }
        //补满后就输出
        Tuple res_tuple = merge_tuples(temp_tuples, select_order);
        if (match_join_condition(&res_tuple, condition_idxs))
          print_tuples.add(std::move(res_tuple));
      }
      //聚合算子
      aggregation_exec(selects, &print_tuples);
      print_tuples.print(ss, true);
    }
  } else {
    // 当前只查询一张表，直接返回结果即可
    //聚合算子
    aggregation_exec(selects, &(tuple_sets.front()));
    tuple_sets.front().print(ss, selects.relation_num > 1);
  }
  for (SelectExeNode *&tmp_node : select_nodes) {
    delete tmp_node;
  }
  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);
  return rc;
}

bool match_table(const Selects &selects, const char *table_name_in_condition,
                 const char *table_name_to_match) {
  if (table_name_in_condition != nullptr) {
    return 0 == strcmp(table_name_in_condition, table_name_to_match);
  }

  return selects.relation_num == 1;
}

static RC schema_add_field(Table *table, const char *field_name,
                           TupleSchema &schema) {
  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta) {
    LOG_WARN("No such field. %s.%s", table->name(), field_name);
    return RC::SCHEMA_FIELD_MISSING;
  }

  schema.add_if_not_exists(field_meta->type(), table->name(),
                           field_meta->name());
  return RC::SUCCESS;
}

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
RC create_selection_executor(Trx *trx, const Selects &selects, const char *db,
                             const char *table_name,
                             SelectExeNode &select_node) {
  Table *table;
  // attribute tables check
  for (size_t i = 0; i < selects.attr_num; i++) {
    if (selects.attributes[i].relation_name == nullptr) {
      continue;
    }
    table = DefaultHandler::get_default().find_table(
        db, selects.attributes[i].relation_name);
    if (nullptr == table) {
      LOG_WARN("No such table [%s] in db [%s]",
               selects.attributes[i].relation_name, db);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
  }

  // condition tables check
  for (size_t i = 0; i < selects.condition_num; i++) {
    if (selects.conditions[i].left_is_attr == 1) {
      if (selects.conditions[i].left_attr.relation_name == nullptr) {
        continue;
      }
      table = DefaultHandler::get_default().find_table(
          db, selects.conditions[i].left_attr.relation_name);
      if (nullptr == table) {
        LOG_WARN("No such table [%s] in db [%s]",
                 selects.conditions[i].left_attr.relation_name, db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
    }
    if (selects.conditions[i].right_is_attr == 1) {
      if (selects.conditions[i].right_attr.relation_name == nullptr) {
        continue;
      }
      table = DefaultHandler::get_default().find_table(
          db, selects.conditions[i].right_attr.relation_name);
      if (nullptr == table) {
        LOG_WARN("No such table [%s] in db [%s]",
                 selects.conditions[i].right_attr.relation_name, db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
    }
  }

  // 列出跟这张表关联的Attr
  TupleSchema schema;
  table = DefaultHandler::get_default().find_table(db, table_name);
  if (nullptr == table) {
    LOG_WARN("No such table [%s] in db [%s]", table_name, db);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  //如果是聚合函数：count/min/max/avg(PARAMETER)，直接select PARAMETER
  if (selects.aggregation_num > 0 && selects.attr_num == 0) {
    for (int i = selects.aggregation_num - 1; i >= 0; i--) {
      if (1 == selects.aggregations[i].is_value) {
        // 列出这张表所有字段
        TupleSchema::from_table(table, schema);
        break;  // 没有校验，给出* 之后，再写字段的错误
      }
      const RelAttr &attr = selects.aggregations[i].attribute;
      if (nullptr == attr.relation_name ||
          0 == strcmp(table_name, attr.relation_name)) {
        if (0 == strcmp("*", attr.attribute_name)) {
          // 列出这张表所有字段
          TupleSchema::from_table(table, schema);
          break;  // 没有校验，给出* 之后，再写字段的错误
        } else {
          // 列出这张表相关字段
          RC rc = schema_add_field(table, attr.attribute_name, schema);
          if (rc != RC::SUCCESS) {
            return rc;
          }
        }
      }
    }
  } else {  // 正常的投影操作
    for (int i = selects.attr_num - 1; i >= 0; i--) {
      const RelAttr &attr = selects.attributes[i];
      if (nullptr == attr.relation_name ||
          0 == strcmp(table_name, attr.relation_name)) {
        if (0 == strcmp("*", attr.attribute_name)) {
          // 列出这张表所有字段
          TupleSchema::from_table(table, schema);
          break;  // 没有校验，给出* 之后，再写字段的错误
        } else {
          // 列出这张表相关字段
          RC rc = schema_add_field(table, attr.attribute_name, schema);
          if (rc != RC::SUCCESS) {
            return rc;
          }
        }
      }
    }
  }

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    if ((condition.left_is_attr == 0 &&
         condition.right_is_attr == 0) ||  // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 &&
         match_table(selects, condition.left_attr.relation_name,
                     table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 &&
         match_table(selects, condition.right_attr.relation_name,
                     table_name)) ||  // 左边是值，右边是属性名
        (condition.left_is_attr == 1 && condition.right_is_attr == 1 &&
         match_table(selects, condition.left_attr.relation_name, table_name) &&
         match_table(selects, condition.right_attr.relation_name,
                     table_name))  // 左右都是属性名，并且表名都符合
    ) {
      DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
      RC rc = condition_filter->init(*table, condition);
      if (rc != RC::SUCCESS) {
        delete condition_filter;
        for (DefaultConditionFilter *&filter : condition_filters) {
          delete filter;
        }
        return rc;
      }
      condition_filters.push_back(condition_filter);
    }
  }

  return select_node.init(trx, table, std::move(schema),
                          std::move(condition_filters));
}
//-----------都让开，我要重构了-----------
void select_nodes_destroy(std::vector<SelectExeNode *> &select_nodes) {
  for (SelectExeNode *&tmp_node : select_nodes) {
    delete tmp_node;
  }
}
RC check_select_node_meta(const Selects &selects, const char *db,
                          const char *table_name, Table *table) {
  // attribute tables check
  for (size_t i = 0; i < selects.attr_num; i++) {
    if (selects.attributes[i].relation_name == nullptr) {
      continue;
    }
    table = DefaultHandler::get_default().find_table(
        db, selects.attributes[i].relation_name);
    if (nullptr == table) {
      LOG_WARN("No such table [%s] in db [%s]",
               selects.attributes[i].relation_name, db);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
  }

  // condition tables check
  for (size_t i = 0; i < selects.condition_num; i++) {
    if (selects.conditions[i].left_is_attr == 1) {
      if (selects.conditions[i].left_attr.relation_name == nullptr) {
        continue;
      }
      table = DefaultHandler::get_default().find_table(
          db, selects.conditions[i].left_attr.relation_name);
      if (nullptr == table) {
        LOG_WARN("No such table [%s] in db [%s]",
                 selects.conditions[i].left_attr.relation_name, db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
    }
    if (selects.conditions[i].right_is_attr == 1) {
      if (selects.conditions[i].right_attr.relation_name == nullptr) {
        continue;
      }
      table = DefaultHandler::get_default().find_table(
          db, selects.conditions[i].right_attr.relation_name);
      if (nullptr == table) {
        LOG_WARN("No such table [%s] in db [%s]",
                 selects.conditions[i].right_attr.relation_name, db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
    }
  }
  table = DefaultHandler::get_default().find_table(db, table_name);
  if (nullptr == table) {
    LOG_WARN("No such table [%s] in db [%s]", table_name, db);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  return RC::SUCCESS;
}
RC create_selection_executor_v2(Trx *trx, const Selects &selects,
                                const char *db, const char *table_name,
                                SelectExeNode &select_node) {
  Table *table;
  RC rc;
  rc = check_select_node_meta(selects, db, table_name, table);
  // 列出跟这张表关联的Attr
  TupleSchema schema;

  //如果是聚合函数：count/min/max/avg(PARAMETER)，直接select PARAMETER
  if (selects.aggregation_num > 0 && selects.attr_num == 0) {
    for (int i = selects.aggregation_num - 1; i >= 0; i--) {
      if (1 == selects.aggregations[i].is_value) {
        // 列出这张表所有字段
        TupleSchema::from_table(table, schema);
        break;  // 没有校验，给出* 之后，再写字段的错误
      }
      const RelAttr &attr = selects.aggregations[i].attribute;
      if (nullptr == attr.relation_name ||
          0 == strcmp(table_name, attr.relation_name)) {
        if (0 == strcmp("*", attr.attribute_name)) {
          // 列出这张表所有字段
          TupleSchema::from_table(table, schema);
          break;  // 没有校验，给出* 之后，再写字段的错误
        } else {
          // 列出这张表相关字段
          RC rc = schema_add_field(table, attr.attribute_name, schema);
          if (rc != RC::SUCCESS) {
            return rc;
          }
        }
      }
    }
  } else {  // 正常的投影操作
    for (int i = selects.attr_num - 1; i >= 0; i--) {
      const RelAttr &attr = selects.attributes[i];
      if (nullptr == attr.relation_name ||
          0 == strcmp(table_name, attr.relation_name)) {
        if (0 == strcmp("*", attr.attribute_name)) {
          // 列出这张表所有字段
          TupleSchema::from_table(table, schema);
          break;  // 没有校验，给出* 之后，再写字段的错误
        } else {
          // 列出这张表相关字段
          RC rc = schema_add_field(table, attr.attribute_name, schema);
          if (rc != RC::SUCCESS) {
            return rc;
          }
        }
      }
    }
  }

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    if ((condition.left_is_attr == 0 &&
         condition.right_is_attr == 0) ||  // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 &&
         match_table(selects, condition.left_attr.relation_name,
                     table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 &&
         match_table(selects, condition.right_attr.relation_name,
                     table_name)) ||  // 左边是值，右边是属性名
        (condition.left_is_attr == 1 && condition.right_is_attr == 1 &&
         match_table(selects, condition.left_attr.relation_name, table_name) &&
         match_table(selects, condition.right_attr.relation_name,
                     table_name))  // 左右都是属性名，并且表名都符合
    ) {
      DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
      RC rc = condition_filter->init(*table, condition);
      if (rc != RC::SUCCESS) {
        delete condition_filter;
        for (DefaultConditionFilter *&filter : condition_filters) {
          delete filter;
        }
        return rc;
      }
      condition_filters.push_back(condition_filter);
    }
  }

  return select_node.init(trx, table, std::move(schema),
                          std::move(condition_filters));
}
void aggregation_exec_v2(const Selects &selects, TupleSet &res_tuples) {
  if (selects.aggregation_num > 0) {
    //目前agg只涉及单表情况
    const char *table_name = selects.relations[0];
    //先设置schema
    TupleSchema agg_schema;
    for (size_t i = 0; i < selects.aggregation_num; i++) {
      const Aggregation &agg = selects.aggregations[i];
      //设置type
      AttrType attr;
      switch (agg.func_name) {
        case FuncName::AGG_MAX:
        case FuncName::AGG_MIN: {
          if (agg.is_value) {
            attr = agg.value.type;
          } else {
            //获取对应的field
            const TupleSchema &ts = res_tuples.get_schema();
            const TupleField &tf = ts.field(ts.index_of_field(
                table_name,
                agg.attribute.attribute_name));  //获取func(age)的age
            attr = tf.type();
          }

          break;
        }

        case FuncName::AGG_COUNT:
          attr = AttrType::INTS;
          break;
        case FuncName::AGG_AVG:
          attr = AttrType::FLOATS;
          break;
      }

      agg_schema.add(attr, table_name, agg_to_string(agg).c_str());
    }
    //再依次添加字段值
    Tuple out;
    for (size_t i = 0; i < selects.aggregation_num; i++) {
      const Aggregation &agg = selects.aggregations[i];
      const std::vector<Tuple> &tuples = res_tuples.tuples();
      switch (agg.func_name) {
        case FuncName::AGG_MAX:
        case FuncName::AGG_MIN: {
          int type = agg.func_name == FuncName::AGG_MAX ? 1 : -1;

          //遍历所有元组，获取最大值
          int target_idx = 0;
          for (size_t t = 1; t < tuples.size(); t++) {
            if (type * tuples[t]
                           .get(res_tuples.get_schema().index_of_field(
                               table_name, agg.attribute.attribute_name))
                           .compare(tuples[target_idx].get(
                               res_tuples.get_schema().index_of_field(
                                   table_name, agg.attribute.attribute_name))) >
                0) {
              target_idx = t;
            }
          }
          //增加这条记录
          out.add(tuples[target_idx].get_pointer(
              res_tuples.get_schema().index_of_field(
                  table_name, agg.attribute.attribute_name)));

          break;
        }
        case FuncName::AGG_COUNT: {
          // 值为size的大小
          //增加这条记录
          out.add((int)tuples.size());
          break;
        }
        case FuncName::AGG_AVG: {
          //遍历所有元组，获取和
          float sum = 0;
          for (size_t t = 0; t < tuples.size(); t++) {
            AttrType type = tuples[t]
                                .get(res_tuples.get_schema().index_of_field(
                                    table_name, agg.attribute.attribute_name))
                                .get_type();
            switch (type) {
              case AttrType::INTS:
                sum += ((IntValue &)tuples[t].get(
                            res_tuples.get_schema().index_of_field(
                                table_name, agg.attribute.attribute_name)))
                           .get_value();
                break;
              case AttrType::FLOATS:
                sum += ((FloatValue &)tuples[t].get(
                            res_tuples.get_schema().index_of_field(
                                table_name, agg.attribute.attribute_name)))
                           .get_value();
                break;
              case AttrType::DATES:
                sum += ((DateValue &)tuples[t].get(
                            res_tuples.get_schema().index_of_field(
                                table_name, agg.attribute.attribute_name)))
                           .get_value();
                break;
              default:
                // TODO: 报错，非数值类型
                break;
            }
          }
          //增加这条记录
          out.add(sum / tuples.size());
          break;
        }
      }
    }
    //等所有值都计算完再去清除res
    res_tuples.clear();
    res_tuples.set_schema(agg_schema);
    res_tuples.add(std::move(out));
  }

  return;
}

RC generate_select_node(const Selects &selects, Session *session, Trx *trx,
                        const char *db,
                        std::vector<SelectExeNode *> &select_nodes) {
  RC rc = RC::SUCCESS;
  for (size_t i = 0; i < selects.relation_num; i++) {
    const char *table_name = selects.relations[i];
    SelectExeNode *select_node = new SelectExeNode;
    rc = create_selection_executor_v2(trx, selects, db, table_name,
                                      *select_node);
    if (rc != RC::SUCCESS) {
      delete select_node;
      for (SelectExeNode *&tmp_node : select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    }
    select_nodes.push_back(select_node);
  }
  if (select_nodes.empty()) {
    LOG_ERROR("No table given");
    end_trx_if_need(session, trx, false);
    return RC::SQL_SYNTAX;
  }
  return rc;
}
RC exec_select_node(Session *session, Trx *trx,
                    std::vector<SelectExeNode *> &select_nodes,
                    std::vector<TupleSet> &tuple_sets) {
  RC rc = RC::SUCCESS;
  for (SelectExeNode *&node : select_nodes) {
    TupleSet tuple_set;
    rc = node->execute(tuple_set);
    if (rc != RC::SUCCESS) {
      select_nodes_destroy(select_nodes);
      end_trx_if_need(session, trx, false);
      return rc;
    } else {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }
  return rc;
}
void multi_table_select(const Selects &selects,
                        const std::vector<TupleSet> &tuple_sets,
                        TupleSet &print_tuples) {
  // 本次查询了多张表，需要做join操作
  // 【每个输出元组的列名需要扩展为[tableName].[colName]的形式】
  // 测试环境会保证多表查询的每个attr都是*或table.id的格式，不会出现单单的id
  // 需要按照selects.attribute的顺序添加，如果是t1.*就添加整张表
  TupleSchema join_schema;
  //仅仅是把每个表的每个字段无序拼接起来，用于下面的对应和查找
  TupleSchema old_schema;
  for (std::vector<TupleSet>::const_reverse_iterator rit = tuple_sets.rbegin(),
                                                     rend = tuple_sets.rend();
       rit != rend; ++rit) {
    //这里是某张表投影完的所有字段，如果是select * from t1,t2;
    // old_schema=[t1.a, t1.b, t2.a, t2.b]
    old_schema.append(rit->get_schema());
  }

  //对照着列名的输出顺序。之后输出一行tuple的时候，tuple.col[i]就输出到select_order[i]的位置
  std::vector<int> select_order;
  for (int i = selects.attr_num - 1; i >= 0; i--) {
    const RelAttr &rel_attr = selects.attributes[i];
    if (0 == strcmp("*", rel_attr.attribute_name)) {
      if (rel_attr.relation_name == nullptr) {
        //如果是select * ，添加所有字段
        for (size_t f = 0; f < old_schema.fields().size(); f++) {
          join_schema.add(old_schema.fields()[f]);
          select_order.push_back(f);
        }
      } else {
        //如果是select t1.*，表名匹配的加入字段
        for (size_t f = 0; f < old_schema.fields().size(); f++) {
          if (0 == strcmp(old_schema.fields()[f].table_name(),
                          rel_attr.relation_name)) {
            join_schema.add(old_schema.fields()[f]);
            select_order.push_back(f);
          }
        }
      }
    } else {
      //如果是select t1.age，表名+字段名匹配的加入字段
      int f = old_schema.index_of_field(rel_attr.relation_name,
                                        rel_attr.attribute_name);
      join_schema.add(old_schema.fields()[f]);
      select_order.push_back(f);
    }
  }

  print_tuples.set_schema(join_schema);
  // 【联查的conditions需要找到对应的表】
  // C x 3数组
  // 每一条的3个元素代表（左值的属性在新schema的下标，CompOp运算符，右值的属性在新schema的下标）
  std::vector<std::vector<int>> condition_idxs;
  for (size_t i = 0; i < selects.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    if (condition.left_is_attr == 1 &&
        condition.right_is_attr == 1)  // 左右都是属性名，并且表名都符合
    {
      std::vector<int> temp_con;
      const char *l_table_name = condition.left_attr.relation_name;
      const char *l_field_name = condition.left_attr.attribute_name;
      const CompOp comp = condition.comp;
      const char *r_table_name = condition.right_attr.relation_name;
      const char *r_field_name = condition.right_attr.attribute_name;
      temp_con.push_back(
          print_tuples.get_schema().index_of_field(l_table_name, l_field_name));
      temp_con.push_back(comp);
      temp_con.push_back(
          print_tuples.get_schema().index_of_field(r_table_name, r_field_name));
      condition_idxs.push_back(temp_con);
    }
  }
  // 【元组的拼接需要实现笛卡尔积】
  // 给每个表分配一个pos指针，另外还需要一个输出用的temp_tuple
  std::vector<std::vector<Tuple>::const_iterator> tuple_poses;
  std::vector<std::vector<Tuple>::const_iterator> tuple_poses_begin;
  std::vector<std::vector<Tuple>::const_iterator> tuple_poses_end;

  std::vector<std::vector<Tuple>::const_iterator> temp_tuples;
  for (std::vector<TupleSet>::const_reverse_iterator rit = tuple_sets.rbegin(),
                                                     rend = tuple_sets.rend();
       rit != rend; ++rit) {
    tuple_poses.push_back(rit->tuples().begin());
    tuple_poses_begin.push_back(rit->tuples().begin());
    tuple_poses_end.push_back(rit->tuples().end());
    temp_tuples.push_back(rit->tuples().begin());
  }
  const size_t N = tuple_sets.size();
  //每个表都有数据才会有结果集，一旦有空表就是空结果
  if (temp_tuples.size() == N) {
    //补满后就输出
    Tuple res_tuple = merge_tuples(temp_tuples, select_order);
    if (match_join_condition(&res_tuple, condition_idxs))
      print_tuples.add(std::move(res_tuple));
    while (tuple_poses[0] != tuple_poses_end[0]) {
      //弹出最末的Tuple
      temp_tuples.pop_back();
      //前进一步指针，如果越界，就回到表头(除了结束)，并继续弹出Tuple
      tuple_poses[temp_tuples.size()]++;
      if (tuple_poses[temp_tuples.size()] ==
          tuple_poses_end[temp_tuples.size()]) {
        if (temp_tuples.size() > 0)
          tuple_poses[temp_tuples.size()] =
              tuple_poses_begin[temp_tuples.size()];
        continue;
      }
      //将元组补满
      while (temp_tuples.size() < N) {
        temp_tuples.push_back(tuple_poses[temp_tuples.size()]);
      }
      //补满后就输出
      Tuple res_tuple = merge_tuples(temp_tuples, select_order);
      if (match_join_condition(&res_tuple, condition_idxs))
        print_tuples.add(std::move(res_tuple));
    }
  }
}
RC ExecuteStage::icy_do_select(const char *db, const Query *sql,
                               SessionEvent *session_event) {
  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  const Selects &selects = sql->sstr.selection;
  std::vector<SelectExeNode *> select_nodes;
  //每张表的投影操作，选取的范围包括属于此表的【attribute】【condition中的attribute】
  //生成待执行投影Node
  rc = generate_select_node(selects, session, trx, db, select_nodes);
  if (rc != RC::SUCCESS) return rc;
  //物化到内存，执行N个表的投影，生成N个tuple_set
  std::vector<TupleSet> tuple_sets;
  rc = exec_select_node(session, trx, select_nodes, tuple_sets);
  if (rc != RC::SUCCESS) return rc;

  std::stringstream ss;
  TupleSet print_tuples;
  if (tuple_sets.size() > 1) {
    multi_table_select(selects, tuple_sets, print_tuples);
    //聚合算子
    aggregation_exec_v2(selects, print_tuples);
    print_tuples.print(ss, selects.relation_num > 1);
  } else {
    //聚合算子
    aggregation_exec_v2(selects, tuple_sets.front());
    tuple_sets.front().print(ss, selects.relation_num > 1);
  }

  select_nodes_destroy(select_nodes);
  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);
  return rc;
}
//----火山模型----
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
      //      auto hash_join_plan = dynamic_cast<const HashJoinPlanNode
      //      *>(plan); auto left = CreateExecutor(exec_ctx,
      //      hash_join_plan->GetLeftPlan()); auto right =
      //      CreateExecutor(exec_ctx, hash_join_plan->GetRightPlan()); return
      //      std::make_unique<HashJoinExecutor>(exec_ctx, hash_join_plan,
      //      std::move(left), std::move(right));
      return nullptr;
    }
      // Create a new aggregation executor.
      // case PlanType::Aggregation: {
      //   auto agg_plan = dynamic_cast<const AggregationPlanNode *>(plan);
      //   auto child_executor =
      //       ExecutorFactory::CreateExecutor(exec_ctx,
      //       agg_plan->GetChildPlan());
      //   return std::make_unique<AggregationExecutor>(exec_ctx, agg_plan,
      //                                                std::move(child_executor));
      // }

      // case PlanType::NestedLoopJoin: {
      //   auto nested_loop_join_plan =
      //       dynamic_cast<const NestedLoopJoinPlanNode *>(plan);
      //   auto left =
      //       CreateExecutor(exec_ctx, nested_loop_join_plan->GetLeftPlan());
      //   auto right =
      //       CreateExecutor(exec_ctx, nested_loop_join_plan->GetRightPlan());
      //   return std::make_unique<NestedLoopJoinExecutor>(
      //       exec_ctx, nested_loop_join_plan, std::move(left),
      //       std::move(right));
      // }

    default: {
      assert(false && "No Plan Type");
    }
  }
}
const AbstractExpression *MakeColumnValueExpression(
    const TupleSchema &schema, size_t tuple_idx, const std::string &col_name,
    std::vector<std::unique_ptr<AbstractExpression>> &expressions) {
  size_t col_idx = schema.GetColIdx(col_name);
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
void MakeOutputSchema(
    const std::vector<std::pair<std::string, const AbstractExpression *>>
        &exprs,
    TupleSchema &schema, const char *table_name) {
  for (const auto &input : exprs) {
    int dot = input.first.find('.');
    std::string field_name =
        dot == std::string::npos ? input.first : input.first.substr(dot + 1);
    schema.add(TupleField{input.second->GetReturnType(), table_name,
                          field_name.c_str(), input.second});
  }
}

const AbstractExpression *ParseConditionToExpression(
    const RelAttr rel, const Value val, const int is_attr,
    std::unordered_map<std::string, const Table *> from_tables_map,
    std::vector<std::unique_ptr<AbstractExpression>> &expressions) {
  if (is_attr == 1) {
    std::string table_name;
    if (rel.relation_name == nullptr) {
      table_name = from_tables_map[""]->name();
    } else {
      table_name = rel.relation_name;
    }
    TupleSchema schema;
    TupleSchema::from_table(from_tables_map[table_name], schema);
    return MakeColumnValueExpression(schema, 0, rel.attribute_name,
                                     expressions);
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
        serialize_date(&d, (const char *)val.data);
        return MakeConstantValueExpression(std::make_shared<DateValue>(d),
                                           expressions);
      }
      case AttrType::CHARS: {
        return MakeConstantValueExpression(
            std::make_shared<StringValue>((const char *)val.data), expressions);
      }
      default:
        // TODO: 报错，非数值类型
        return nullptr;
    }
  }
}
RC ExecuteStage::volcano_do_select(const char *db, const Query *sql,
                                   SessionEvent *session_event) {
  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  std::stringstream ss;

  // build query plan
  //全局变量
  std::vector<std::unique_ptr<AbstractExpression>> allocated_expressions;
  // 1. parse FROM
  std::vector<Table *> from_tables;
  for (size_t i = 0; i < sql->sstr.selection.relation_num; i++) {
    Table *table;
    table = DefaultHandler::get_default().find_table(
        db, sql->sstr.selection.relations[i]);
    // from table check
    if (table == nullptr) {
      LOG_WARN("No such table [%s] in db [%s]",
               sql->sstr.selection.relations[i], db);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    from_tables.emplace_back(table);
  }
  // 生成哈希表{表名,表指针}
  std::unordered_map<std::string, const Table *> from_tables_map;
  for (size_t i = 0; i < from_tables.size(); i++) {
    from_tables_map[from_tables[i]->name()] = from_tables[i];
  }
  from_tables_map[""] = from_tables[0];
  // 2. sort from_tables by table length
  std::sort(from_tables.begin(), from_tables.end(),
            [](const Table *t1, const Table *t2) {
              return t1->table_meta().record_size() <
                     t2->table_meta().record_size();
            });
  // 3. parse WHERE
  // std::unordered_map<const char *, std::set<char *>> where_join;
  // std::unordered_map<const char *, std::vector<Condition>> where_comp;
  // for (size_t i = 0; i < sql->sstr.selection.condition_num; i++) {
  //   Condition con;
  //   con = sql->sstr.selection.conditions[i];
  //   if (con.left_is_attr == 1 && con.right_is_attr == 1) {
  //     where_join[con.left_attr.relation_name].insert(
  //         con.left_attr.attribute_name);
  //     where_join[con.right_attr.relation_name].insert(
  //         con.right_attr.attribute_name);
  //   } else if (con.left_is_attr == 1) {
  //     where_comp[con.left_attr.relation_name].push_back(con);
  //   } else {
  //     where_comp[con.right_attr.relation_name].push_back(con);
  //   }
  // }
  std::vector<const AbstractExpression *> predicates;  //默认全是用AND连接的
  for (size_t i = 0; i < sql->sstr.selection.condition_num; i++) {
    Condition con = sql->sstr.selection.conditions[i];

    // 对 DATE 类型进行特殊处理
    if (con.left_is_attr) {
      // condition table check
      if (con.left_attr.relation_name != nullptr &&
          DefaultHandler::get_default().find_table(
              db, con.left_attr.relation_name) == nullptr) {
        LOG_WARN("No such table [%s] in db [%s]", con.left_attr.relation_name,
                 db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      auto rel_name = con.left_attr.relation_name == nullptr
                          ? ""
                          : con.left_attr.relation_name;
      // condition attr check
      if (from_tables_map[rel_name]->table_meta().field(
              con.left_attr.attribute_name) == nullptr) {
        LOG_WARN("No such field. %s.%s", con.left_attr.relation_name,
                 con.left_attr.relation_name);
        return RC::SCHEMA_FIELD_MISSING;
      }
      AttrType left_type = from_tables_map[rel_name]
                               ->table_meta()
                               .field(con.left_attr.attribute_name)
                               ->type();

      if (left_type == DATES && !con.right_is_attr &&
          con.right_value.type == CHARS)
        con.right_value.type = DATES;
    }
    if (con.right_is_attr) {
      // condition table check
      if (con.right_attr.relation_name != nullptr &&
          DefaultHandler::get_default().find_table(
              db, con.right_attr.relation_name) == nullptr) {
        LOG_WARN("No such table [%s] in db [%s]", con.right_attr.relation_name,
                 db);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      auto rel_name = con.right_attr.relation_name == nullptr
                          ? ""
                          : con.right_attr.relation_name;
      // condition attr check
      if (from_tables_map[rel_name]->table_meta().field(
              con.right_attr.attribute_name) == nullptr) {
        LOG_WARN("No such field. %s.%s", con.right_attr.relation_name,
                 con.right_attr.relation_name);
        return RC::SCHEMA_FIELD_MISSING;
      }
      AttrType right_type = from_tables_map[rel_name]
                                ->table_meta()
                                .field(con.right_attr.attribute_name)
                                ->type();
      if (right_type == DATES && !con.left_is_attr &&
          con.left_value.type == CHARS)
        con.left_value.type = DATES;
    }

    const AbstractExpression *lhs = ParseConditionToExpression(
        con.left_attr, con.left_value, con.left_is_attr, from_tables_map,
        allocated_expressions);
    const AbstractExpression *rhs = ParseConditionToExpression(
        con.right_attr, con.right_value, con.right_is_attr, from_tables_map,
        allocated_expressions);
    predicates.push_back(
        MakeComparisonExpression(lhs, rhs, con.comp, allocated_expressions));
  }
  // 4. parse SELECT
  std::vector<std::pair<std::string, const AbstractExpression *>> out_proj;
  for (int i = sql->sstr.selection.attr_num - 1; i >= 0; i--) {
    RelAttr ra;
    ra = sql->sstr.selection.attributes[i];
    const char *table_name = ra.relation_name;
    const char *attr_name = ra.attribute_name;
    // select table check
    if (table_name != nullptr &&
        DefaultHandler::get_default().find_table(db, table_name) == nullptr) {
      LOG_WARN("No such table [%s] in db [%s]", table_name, db);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    // select attr check
    if (strcmp(attr_name, "*") != 0 && table_name != nullptr &&
        from_tables_map[table_name]->table_meta().field(attr_name) != nullptr) {
      LOG_WARN("No such field. %s.%s", table_name, attr_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    //生成字段的输出格式
    std::string out_str;

    if (table_name == nullptr) {
      //单表的tablename全部等于FROM的那个表
      table_name = from_tables[0]->name();
      // 输出列名时没有表名
    } else {
      // 输出："TableA."
      out_str = std::string(table_name) + ".";
    }
    //找到对应的表
    int idx = 0;
    for (; idx < from_tables.size(); idx++) {
      if (0 == strcmp(from_tables[idx]->name(), table_name)) {
        break;
      }
    }
    if (idx == from_tables.size()) {
      LOG_ERROR("有空处理一下报错，没找到表");
    }
    const Table *current_table = from_tables[idx];
    TupleSchema current_schema;
    TupleSchema::from_table(current_table, current_schema);
    if (0 == strcmp("*", attr_name)) {
      // "*" 代表映射所有字段
      for (int f = 0; f < current_schema.fields().size(); f++) {
        const char *temp_attr_name = current_schema.field(f).field_name();
        out_proj.push_back(
            {out_str + temp_attr_name,
             MakeColumnValueExpression(current_schema, 0, temp_attr_name,
                                       allocated_expressions)});
      }
    } else {
      out_proj.push_back({out_str + attr_name, MakeColumnValueExpression(
                                                   current_schema, 0, attr_name,
                                                   allocated_expressions)});
    }
  }

  /**
   *
   * 1 table
   *
   */
  TupleSchema scan_schema;
  std::unique_ptr<AbstractPlanNode> scan_plan;
  Table *scan_table = from_tables[0];
  // for (int i = 0; i < where_comp[scan_table->name()].size(); i++) {
  //   Condition cond = where_comp[scan_table->name()][i];
  //   void *col_name;
  //   if (cond.left_is_attr == 0) {
  //     col_name = cond.left_value.data;
  //   } else {
  //     col_name = cond.right_value.data;
  //   }
  //   if (select_proj[scan_table->name()].find((char *)col_name) ==
  //       select_proj[scan_table->name()].end()) {
  //     const AbstractExpression *col = MakeColumnValueExpression(
  //         schema, 0, (char *)col_name, allocated_expressions);
  //     exprs.emplace_back(std::make_pair<>((char *)col_name, col));
  //   }
  // }
  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  const Selects &selects = sql->sstr.selection;
  const char *table_name = scan_table->name();
  for (size_t i = 0; i < sql->sstr.selection.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    if ((condition.left_is_attr == 0 &&
         condition.right_is_attr == 0) ||  // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 &&
         match_table(selects, condition.left_attr.relation_name,
                     table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 &&
         match_table(selects, condition.right_attr.relation_name,
                     table_name)) ||  // 左边是值，右边是属性名
        (condition.left_is_attr == 1 && condition.right_is_attr == 1 &&
         match_table(selects, condition.left_attr.relation_name, table_name) &&
         match_table(selects, condition.right_attr.relation_name,
                     table_name))  // 左右都是属性名，并且表名都符合
    ) {
      DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
      RC rc = condition_filter->init(*scan_table, condition);
      if (rc != RC::SUCCESS) {
        delete condition_filter;
        for (DefaultConditionFilter *&filter : condition_filters) {
          delete filter;
        }
        return rc;
      }
      condition_filters.push_back(condition_filter);
    }
  }
  MakeOutputSchema(out_proj, scan_schema, scan_table->name());
  scan_schema.print(ss, false);
  scan_plan = std::make_unique<SeqScanPlanNode>(&scan_schema,
                                                scan_table->name(), predicates);

  // execute plan
  ExecutorContext exec_ctx{trx, db};
  auto executor = CreateExecutor(&exec_ctx, scan_plan.get());

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

/**
 *
 * multi tables
 *
 */
// 4. get first left table and construct leaf node for the first join operator
// TupleSchema *left_schema;
// std::unique_ptr<AbstractPlanNode> left_plan;
// {
//   Table *left_table = from_tables[0];
//   TupleSchema schema1;
//   TupleSchema::from_table(left_table, schema1);
//   std::vector<std::pair<std::string, const AbstractExpression *>> exprs1;
//   std::set<char *> join_cond = where_join[left_table->name()];
//   for (std::set<char *>::iterator itr =
//   where_join[left_table->name()].begin(); itr !=
//   where_join[left_table->name()].end(); itr++) {
//     std::vector<std::unique_ptr<AbstractExpression>> allocated_expressions;
//     const AbstractExpression *col = MakeColumnValueExpression(schema1, 0,
//     *itr, allocated_expressions); exprs1.emplace_back(std::make_pair<>(*itr,
//     col));
//   }
//   // for (std::set<char *>::iterator itr =
//   where_simple[left_table->name()].begin(); itr !=
//   where_simple[left_table->name()].end(); itr++) {
//   //   std::vector<std::unique_ptr<AbstractExpression>>
//   allocated_expressions;
//   //   const AbstractExpression *col = MakeColumnValueExpression(schema1, 0,
//   *itr, allocated_expressions);
//   //   exprs1.emplace_back(std::make_pair<>(*itr, col));
//   // }
//   MakeOutputSchema(exprs1, *left_schema, left_table->name());
//   left_plan = std::make_unique<SeqScanPlanNode>(left_schema,
//   left_table->name());
// }

// // right leaf
// TupleSchema *right_schema;
// std::unique_ptr<AbstractPlanNode> right_plan;
// {
//   Table *right_table = from_tables[1];
//   TupleSchema schema2;
//   TupleSchema::from_table(right_table, schema2);
//   std::vector<std::pair<std::string, const AbstractExpression *>> exprs2;
//   std::set<char *> join_cond = where_join[right_table->name()];
//   for (std::set<char *>::iterator itr =
//   where_join[right_table->name()].begin(); itr !=
//   where_join[right_table->name()].end(); itr++) {
//     std::vector<std::unique_ptr<AbstractExpression>> allocated_expressions;
//     const AbstractExpression *col = MakeColumnValueExpression(schema2, 0,
//     *itr, allocated_expressions); exprs2.emplace_back(std::make_pair<>(*itr,
//     col));
//   }
//   // for (std::set<char *>::iterator itr =
//   where_simple[right_table->name()].begin(); itr !=
//   where_simple[right_table->name()].end(); itr++) {
//   //   std::vector<std::unique_ptr<AbstractExpression>>
//   allocated_expressions;
//   //   const AbstractExpression *col = MakeColumnValueExpression(schema2, 0,
//   *itr, allocated_expressions);
//   //   exprs2.emplace_back(std::make_pair<>(*itr, col));
//   // }
//   MakeOutputSchema(exprs2, *right_schema, right_table->name());
//   right_plan = std::make_unique<SeqScanPlanNode>(right_schema,
//   right_table->name());
// }

// // join
// TupleSchema *out_schema;
// std::unique_ptr<HashJoinPlanNode> join_plan;
// {
//   std::vector<std::pair<std::string, const AbstractExpression *>> exprs;

//   // left join
//   // const AbstractExpression *left_col;
//   for (std::set<char *>::iterator itr =
//   where_join[from_tables[0]->name()].begin(); itr !=
//   where_join[from_tables[0]->name()].end(); itr++) {
//     std::vector<std::unique_ptr<AbstractExpression>> allocated_expressions;
//     auto *col = MakeColumnValueExpression(*left_schema, 0, *itr,
//     allocated_expressions); exprs.emplace_back(std::make_pair<>(*itr, col));
//   }

//   // right join
//   // const AbstractExpression *right_col;
//   Table *right_table = from_tables[1];
//   TupleSchema right_schema;
//   for (std::set<char *>::iterator itr =
//   where_join[right_table->name()].begin(); itr !=
//   where_join[right_table->name()].end(); itr++) {
//     std::vector<std::unique_ptr<AbstractExpression>> allocated_expressions;
//     auto *col = MakeColumnValueExpression(right_schema, 1, *itr,
//     allocated_expressions); exprs.emplace_back(std::make_pair<>(*itr, col));
//   }
//   // for (std::set<char *>::iterator itr =
//   where_simple[right_table->name()].begin(); itr !=
//   where_simple[right_table->name()].end(); itr++) {
//   //   std::vector<std::unique_ptr<AbstractExpression>>
//   allocated_expressions;
//   //   const AbstractExpression *col = MakeColumnValueExpression(schema2, 0,
//   *itr, allocated_expressions);
//   //   exprs2.emplace_back(std::make_pair<>(*itr, col));
//   // }
//   MakeOutputSchema(exprs, *out_schema, right_table->name());
//   join_plan = std::make_unique<HashJoinPlanNode>(
//     out_schema, std::vector<const AbstractPlanNode *>{left_plan.get(),
//     right_plan.get()}, exprs[0].second, exprs[1].second);
// }

// // 5. construct other join tables
// // std::unique_ptr<AbstractPlanNode> left_plan{};
// // TupleSchema *left_schema;
// // Table *left_table;
// // left_plan = std::move(scan_plan1);
// // left_schema = out_schema1;
// // left_table = from_tables[0];
// // for (int i = 1; i < from_tables.size(); i++) {
// //   // right leaf
// //   TupleSchema *out_schema2{};
// //   std::unique_ptr<AbstractPlanNode> scan_plan2{};
// //   {
// //     Table *right_table = from_tables[i];
// //     TupleSchema schema2;
// //     TupleSchema::from_table(right_table, schema2);
// //     std::vector<std::pair<std::string, const AbstractExpression *>>
// exprs2;
// //     std::set<char *> join_cond = where_join[right_table->name()];
// //     for (std::set<char *>::iterator itr =
// where_join[right_table->name()].begin(); itr !=
// where_join[right_table->name()].end(); itr++) {
// //       std::vector<std::unique_ptr<AbstractExpression>>
// allocated_expressions;
// //       const AbstractExpression *col = MakeColumnValueExpression(schema2,
// 0, *itr, allocated_expressions);
// //       exprs2.emplace_back(std::make_pair<>(*itr, col));
// //     }
// //     // for (std::set<char *>::iterator itr =
// where_simple[right_table->name()].begin(); itr !=
// where_simple[right_table->name()].end(); itr++) {
// //     //   std::vector<std::unique_ptr<AbstractExpression>>
// allocated_expressions;
// //     //   const AbstractExpression *col =
// MakeColumnValueExpression(schema2, 0, *itr, allocated_expressions);
// //     //   exprs2.emplace_back(std::make_pair<>(*itr, col));
// //     // }
// //     MakeOutputSchema(exprs2, *out_schema2, right_table->name());
// //     scan_plan2 = std::make_unique<SeqScanPlanNode>(out_schema2,
// right_table->name());
// //   }
// //   // join
// //   TupleSchema *out_schema;
// //   std::unique_ptr<HashJoinPlanNode> join_plan;
// //   {
// //     std::vector<std::pair<std::string, const AbstractExpression *>> exprs;

// //     // left join
// //     // const AbstractExpression *left_col;
// //     for (std::set<char *>::iterator itr =
// where_join[left_table->name()].begin(); itr !=
// where_join[left_table->name()].end(); itr++) {
// //       std::vector<std::unique_ptr<AbstractExpression>>
// allocated_expressions;
// //       const AbstractExpression *col =
// MakeColumnValueExpression(*left_schema, 0, *itr, allocated_expressions);
// //       exprs.emplace_back(std::make_pair<>(*itr, col));
// //     }

// //     // right join
// //     // const AbstractExpression *right_col;
// //     Table *right_table = from_tables[i];
// //     TupleSchema right_schema;
// //     for (std::set<char *>::iterator itr =
// where_join[right_table->name()].begin(); itr !=
// where_join[right_table->name()].end(); itr++) {
// //       std::vector<std::unique_ptr<AbstractExpression>>
// allocated_expressions;
// //       const AbstractExpression *col =
// MakeColumnValueExpression(right_schema, 0, *itr, allocated_expressions);
// //       exprs.emplace_back(std::make_pair<>(*itr, col));
// //     }
// //     // for (std::set<char *>::iterator itr =
// where_simple[right_table->name()].begin(); itr !=
// where_simple[right_table->name()].end(); itr++) {
// //     //   std::vector<std::unique_ptr<AbstractExpression>>
// allocated_expressions;
// //     //   const AbstractExpression *col =
// MakeColumnValueExpression(schema2, 0, *itr, allocated_expressions);
// //     //   exprs2.emplace_back(std::make_pair<>(*itr, col));
// //     // }
// //     MakeOutputSchema(exprs, *out_schema, right_table->name());
// //     join_plan = std::make_unique<HashJoinPlanNode>(
// //       out_schema, std::vector<const AbstractPlanNode *>{left_plan.get(),
// scan_plan2.get()}, exprs[0].second, exprs[1].second);
// //   }
// //   left_plan = std::move(join_plan);
// //   left_schema = out_schema;
// //   left_table = from_tables[i];
// // }

// // execute query plan
// ExecutorContext exec_ctx{trx, db};
// auto executor = CreateExecutor(&exec_ctx, join_plan.get());

// executor->Init();

// Tuple tuple;
// RID rid;
// // rid初始值
// rid.page_num = 1;
// rid.slot_num = -1;
// while ((rc = executor->Next(&tuple, &rid)) == RC::SUCCESS) {
//   tuple.print(ss);
// }
// if (rc != RC::EMPTY) {
//   end_trx_if_need(session, trx, false);
//   return rc;
// }
// session_event->set_response(ss.str());
// end_trx_if_need(session, trx, true);

// query plan
// std::vector<std::unique_ptr<AbstractExpression>> allocated_expressions;
// Table *table = DefaultHandler::get_default().find_table(db, "t");
// TupleSchema schema;
// TupleSchema::from_table(table, schema);
// const AbstractExpression *col_a =
//     MakeColumnValueExpression(schema, 0, "id", allocated_expressions);
// TupleSchema output_schema;
// MakeOutputSchema({{"id", col_a}}, output_schema, table->name());
// output_schema.print(ss, false);
// SeqScanPlanNode plan{&output_schema, table->name()};

// ExecutorContext exec_ctx{trx, db};
// std::unique_ptr<AbstractExecutor> executor = CreateExecutor(&exec_ctx,
// &plan);
// // prepare
// executor->Init();

// // execute
// Tuple tuple;
// RID rid;
// // rid初始值
// rid.page_num = 1;
// rid.slot_num = -1;
// while ((rc = executor->Next(&tuple, &rid)) == RC::SUCCESS) {
//   tuple.print(ss);
// }
// // if (rc != RC::EMPTY) {
// //   end_trx_if_need(session, trx, false);
// //   return rc;
// // }
// session_event->set_response(ss.str());
// end_trx_if_need(session, trx, true);
// return RC::SUCCESS;
