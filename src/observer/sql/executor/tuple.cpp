/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its
affiliates. All rights reserved. miniob is licensed under Mulan PSL v2. You can
use this software according to the terms and conditions of the Mulan PSL v2. You
may obtain a copy of Mulan PSL v2 at: http://license.coscl.org.cn/MulanPSL2 THIS
SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2021/5/14.
//

#include "sql/executor/tuple.h"

#include "common/log/log.h"
#include "sql/executor/expressions/column_value_expression.h"
#include "storage/common/record_manager.h"
#include "storage/common/table.h"

Tuple::Tuple(const Tuple &other) {
  LOG_PANIC("Copy constructor of tuple is not supported");
  exit(1);
}

Tuple::Tuple(Tuple &&other)

    noexcept
    : values_(std::move(other.values_)) {}

Tuple &Tuple::operator=(Tuple &&other)

    noexcept {
  if (&other == this) {
    return *this;
  }

  values_.clear();

  values_.swap(other.values_);
  return *this;
}

Tuple::~Tuple() {}

// add (Value && value)
void Tuple::add(TupleValue *value) { values_.emplace_back(value); }
void Tuple::add(const TupleValue *value) {
  values_.emplace_back((TupleValue *)value);
}

void Tuple::add(const std::shared_ptr<TupleValue> &other) {
  values_.emplace_back(other);
}

void Tuple::add() { add(new NullValue()); }

void Tuple::add(int value) { add(new IntValue(value)); }

void Tuple::add(uint16_t value) { add(new DateValue(value)); }

void Tuple::add(float value) { add(new FloatValue(value)); }

void Tuple::add(const char *s, int len) { add(new StringValue(s, len)); }
void Tuple::print(std::ostream &os) {
  if (values_.empty()) return;
  for (std::vector<std::shared_ptr<TupleValue>>::const_iterator
           iter = values_.begin(),
           end = --values_.end();
       iter != end; ++iter) {
    (*iter)->to_string(os);
    os << " | ";
  }
  values_.back()->to_string(os);
  os << std::endl;
}
////////////////////////////////////////////////////////////////////////////////

std::string TupleField::to_string() const {
  return std::string(table_name_) + "." + field_name_ + std::to_string(type_);
}

////////////////////////////////////////////////////////////////////////////////
void TupleSchema::from_table(const Table *table, TupleSchema &schema) {
  const char *table_name = table->name();
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = 0; i < field_num; i++) {
    const FieldMeta *field_meta = table_meta.field(i);
    if (field_meta->visible()) {
      schema.add(field_meta->type(), table_name, field_meta->name());
    }
  }
}

void TupleSchema::add(AttrType type, const char *table_name,
                      const char *field_name) {
  fields_.emplace_back(type, table_name, field_name);
}

void TupleSchema::add(AttrType type, const char *table_name,
                      const char *field_name, const AbstractExpression *expr) {
  fields_.emplace_back(type, table_name, field_name, expr);
}

void TupleSchema::add(const TupleField &otherfield) {
  add(otherfield.type(), otherfield.table_name(), otherfield.field_name(),
      otherfield.expr());
}
void TupleSchema::add_if_not_exists(AttrType type, const char *table_name,
                                    const char *field_name) {
  for (const auto &field : fields_) {
    if (0 == strcmp(field.table_name(), table_name) &&
        0 == strcmp(field.field_name(), field_name)) {
      return;
    }
  }

  add(type, table_name, field_name);
}

void TupleSchema::append(const TupleSchema &other) {
  size_t delta = fields_.size();
  size_t idx = 0;
  fields_.reserve(delta + other.fields_.size());
  for (const auto &field : other.fields_) {
    ColumnValueExpression *old_col = (ColumnValueExpression *)(field.expr());
    ColumnValueExpression *temp_col = new ColumnValueExpression{
        old_col->GetTupleIdx(), idx++, old_col->GetReturnType()};
    TupleField *temp_field = new TupleField(field.type(), field.table_name(),
                                            field.field_name(), temp_col);
    auto col_expr = (ColumnValueExpression *)(temp_field->expr());
    col_expr->AppendColIdx(delta);
    fields_.emplace_back(*temp_field);
  }
}

int TupleSchema::index_of_field(const char *table_name,
                                const char *field_name) const {
  const int size = fields_.size();
  for (int i = 0; i < size; i++) {
    const TupleField &field = fields_[i];
    if (0 == strcmp(field.table_name(), table_name) &&
        0 == strcmp(field.field_name(), field_name)) {
      return i;
    }
  }
  return -1;
}

void TupleSchema::print(std::ostream &os, bool multi_table) const {
  if (fields_.empty()) {
    os << "No schema";
    return;
  }

  // 判断有多张表还是只有一张表
  std::set<std::string> table_names;
  for (const auto &field : fields_) {
    table_names.insert(field.table_name());
  }

  for (std::vector<TupleField>::const_iterator iter = fields_.begin(),
                                               end = --fields_.end();
       iter != end; ++iter) {
    if (multi_table) {
      os << iter->table_name() << ".";
    }
    os << iter->field_name() << " | ";
  }

  if (multi_table) {
    os << fields_.back().table_name() << ".";
  }
  os << fields_.back().field_name() << std::endl;
}
void TupleSchema::printExprs(std::ostream &os) const {
  if (fields_.empty()) {
    os << "No schema";
    return;
  }

  // 判断有多张表还是只有一张表
  std::set<std::string> table_names;
  for (const auto &field : fields_) {
    table_names.insert(field.table_name());
  }

  for (std::vector<TupleField>::const_iterator iter = fields_.begin(),
                                               end = --fields_.end();
       iter != end; ++iter) {
    os << ((ColumnValueExpression *)iter->expr())->GetColIdx() << " @ ";
  }

  os << ((ColumnValueExpression *)fields_.back().expr())->GetColIdx()
     << std::endl;
}
size_t TupleSchema::GetColIdx(const std::string &col_name) const {
  for (size_t i = 0; i < fields_.size(); ++i) {
    if (std::string{fields_[i].field_name()} == col_name) {
      return i;
    }
  }
}

size_t TupleSchema::GetFieldIdx(const std::string &field_name) const {
  for (size_t i = 0; i < fields_.size(); ++i) {
    if (std::string{fields_[i].table_name()} + "." +
            std::string{fields_[i].field_name()} ==
        field_name) {
      return i;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
TupleSet::TupleSet(TupleSet &&other)
    : tuples_(std::move(other.tuples_)), schema_(other.schema_) {
  other.schema_.clear();
}

TupleSet &TupleSet::operator=(TupleSet &&other) {
  if (this == &other) {
    return *this;
  }

  schema_.clear();
  schema_.append(other.schema_);
  other.schema_.clear();

  tuples_.clear();
  tuples_.swap(other.tuples_);
  return *this;
}

void TupleSet::add(Tuple &&tuple) { tuples_.emplace_back(std::move(tuple)); }

void TupleSet::clear() {
  tuples_.clear();
  schema_.clear();
}

void TupleSet::print(std::ostream &os, bool multi_table) const {
  if (schema_.fields().empty()) {
    LOG_WARN("Got empty schema");
    return;
  }

  schema_.print(os, multi_table);

  for (const Tuple &item : tuples_) {
    const std::vector<std::shared_ptr<TupleValue>> &values = item.values();
    for (std::vector<std::shared_ptr<TupleValue>>::const_iterator
             iter = values.begin(),
             end = --values.end();
         iter != end; ++iter) {
      (*iter)->to_string(os);
      os << " | ";
    }
    values.back()->to_string(os);
    os << std::endl;
  }
}

void TupleSet::set_schema(const TupleSchema &schema) { schema_ = schema; }

const TupleSchema &TupleSet::get_schema() const { return schema_; }

bool TupleSet::is_empty() const { return tuples_.empty(); }

int TupleSet::size() const { return tuples_.size(); }

const Tuple &TupleSet::get(int index) const { return tuples_[index]; }

const std::vector<Tuple> &TupleSet::tuples() const { return tuples_; }

/////////////////////////////////////////////////////////////////////////////
TupleRecordConverter::TupleRecordConverter(Table *table, TupleSet &tuple_set)
    : table_(table), tuple_set_(tuple_set) {}

void TupleRecordConverter::add_record(const char *record) {
  const TupleSchema &schema = tuple_set_.schema();
  Tuple tuple;
  const TableMeta &table_meta = table_->table_meta();
  for (const TupleField &field : schema.fields()) {
    const FieldMeta *field_meta = table_meta.field(field.field_name());
    assert(field_meta != nullptr);
    switch (field_meta->type()) {
      case INTS: {
        int value = *(int *)(record + field_meta->offset());
        tuple.add(value);
      } break;
      case DATES: {
        uint16_t value = *(uint16_t *)(record + field_meta->offset());
        tuple.add(value);
      } break;
      case FLOATS: {
        float value = *(float *)(record + field_meta->offset());
        tuple.add(value);
      } break;
      case CHARS: {
        const char *s = record + field_meta->offset();  // 现在当做Cstring来处理
        tuple.add(s, strlen(s));
      } break;
      case TEXTS: {
        int overflow_id = *(int *)(record + field_meta->offset());
        FILE *overflow_file = table_->overflow_file();
        fseek(overflow_file, overflow_id * 4096, SEEK_SET);
        char *buf = new char[4097];
        buf[4096] = 0;
        fread(buf, 4096, 1, overflow_file);
        tuple.add(buf, strlen(buf));
      } break;
      default: {
        LOG_PANIC("Unsupported field type. type=%d", field_meta->type());
      }
    }
  }

  tuple_set_.add(std::move(tuple));
}

void TupleRecordConverter::record_to_tuple(Tuple *tuple, Record *record) {
  const TableMeta &table_meta = table_->table_meta();
  int null_mask_field_offset = table_meta.null_mask_field()->offset();
  int null_mask =
      *(int *)(record->data + null_mask_field_offset) & 0x00FFFFFFFF;
  int sys_field_num = table_meta.sys_field_num();
  for (int i = sys_field_num; i < table_meta.field_num(); i++) {
    if (null_mask >> i & 1) {
      tuple->add();
      continue;
    }
    const FieldMeta *field_meta = table_meta.field(i);
    assert(field_meta != nullptr);
    switch (field_meta->type()) {
      case INTS: {
        int value = *(int *)(record->data + field_meta->offset());
        tuple->add(value);
      } break;
      case DATES: {
        uint16_t value = *(uint16_t *)(record->data + field_meta->offset());
        tuple->add(value);
      } break;
      case FLOATS: {
        float value = *(float *)(record->data + field_meta->offset());
        tuple->add(value);
      } break;
      case CHARS: {
        const char *s =
            record->data + field_meta->offset();  // 现在当做Cstring来处理
        tuple->add(s, strlen(s));
      } break;
      case TEXTS: {
        int overflow_id = *(int *)(record->data + field_meta->offset());
        FILE *overflow_file = table_->overflow_file();
        fseek(overflow_file, overflow_id * 4096, SEEK_SET);
        char *buf = new char[4097];
        buf[4096] = 0;
        fread(buf, 4096, 1, overflow_file);
        tuple->add(buf, strlen(buf));
      } break;
      default: {
        LOG_PANIC("Unsupported field type. type=%d", field_meta->type());
      }
    }
  }
}