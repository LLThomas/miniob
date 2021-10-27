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

#include <sstream>
#include <string>

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
      RC rc =
          do_select(current_db, sql, exe_event->sql_event()->session_event());
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
  if (1 == agg.is_value) {
    AttrType type = agg.value->type;
    void *val = agg.value->data;
    std::string str;
    switch (type) {
      case AttrType::INTS:
        str = std::to_string(*((int *)val));
        break;
      case AttrType::FLOATS:
        str = std::to_string(*((float *)val));
        break;
      case AttrType::DATES:
        str = std::to_string(*((uint16_t *)val));
        break;
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
    //先设置schema
    TupleSchema agg_schema;
    for (size_t i = 0; i < selects.aggregation_num; i++) {
      const Aggregation &agg = selects.aggregations[i];
      const TupleField &tf =
          res_tuples->get_schema().field(i);  //获取func(age)的age
      agg_schema.add(tf.type(), tf.table_name(), agg_to_string(agg).c_str());
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
                           .get(selects.aggregation_num - 1 - i)
                           .compare(tuples[target_idx].get(
                               selects.aggregation_num - 1 - i)) >
                0) {
              target_idx = t;
            }
          }
          //增加这条记录
          out.add(
              tuples[target_idx].get_pointer(selects.aggregation_num - 1 - i));

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
            AttrType type =
                tuples[t].get(selects.aggregation_num - 1 - i).get_type();
            switch (type) {
              case AttrType::INTS:
                sum +=
                    ((IntValue &)tuples[t].get(selects.aggregation_num - 1 - i))
                        .get_value();
                break;
              case AttrType::FLOATS:
                sum += ((FloatValue &)tuples[t].get(selects.aggregation_num -
                                                    1 - i))
                           .get_value();
                break;
              case AttrType::DATES:
                sum += ((DateValue &)tuples[t].get(selects.aggregation_num - 1 -
                                                   i))
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
  size_t order = 0;
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
RC ExecuteStage::do_select(const char *db, Query *sql,
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
        for (size_t f = 0; f < old_schema.fields().size(); f++) {
          if ((0 == strcmp(old_schema.fields()[f].table_name(),
                           rel_attr.relation_name)) &&
              (0 == strcmp(old_schema.fields()[f].field_name(),
                           rel_attr.attribute_name))) {
            join_schema.add(old_schema.fields()[f]);
            select_order.push_back(f);
            break;
          }
        }
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
      print_tuples.print(ss);
    }
  } else {
    // 当前只查询一张表，直接返回结果即可
    //聚合算子
    aggregation_exec(selects, &(tuple_sets.front()));
    tuple_sets.front().print(ss);
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
    if (selects.conditions[i].left_is_attr == 1) {
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
