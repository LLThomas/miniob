#include "sql/executor/executors/hash_join_executor.h"

#include <unordered_map>
#include <string>

std::unordered_map<std::string, std::vector<Tuple *>> hash_map;

HashJoinExecutor::HashJoinExecutor(ExecutorContext *exec_ctx, HashJoinPlanNode *plan,
                                   std::unique_ptr<AbstractExecutor> &&left_child,
                                   std::unique_ptr<AbstractExecutor> &&right_child)
    : AbstractExecutor(exec_ctx),
    plan_(plan),
    left_(std::move(left_child)),
    right_(std::move(right_child)) {}

void HashJoinExecutor::Init() {
    left_->Init();
    // 1. build
    int col_index = 0;
    Tuple tuple;
    // RID rid;
    // rid.page_num = 1;
    // rid.slot_num = -1;
    // while (left_->Next(&tuple, &rid) == RC::SUCCESS) {
    //     std::stringstream ss;
    //     tuple.get(col_index).to_string(ss);
    //     std::string s;
    //     ss >> s;
    //     hash_map[s].emplace_back(tuple);
    // }
    right_->Init();
}

RC HashJoinExecutor::Next(Tuple *tuple, RID *rid) {
    // 2. probe
    int col_index = 0;
    Tuple tuple_;
    RID rid_;
    rid_.page_num = rid->page_num;
    rid_.slot_num = rid->slot_num;
    while (right_->Next(&tuple_, &rid_) == RC::SUCCESS) {
        std::stringstream ss;
        tuple_.get(col_index).to_string(ss);
        std::string s;
        ss >> s;
        if (hash_map.find(s) != hash_map.end()) {
            std::vector<Tuple *> tuples = hash_map[s];
            TupleSet tuple_set;
            tuple_set.clear();
            tuple_set.set_schema(*(plan_->OutputSchema()));
            for (int i = 0; i < tuples.size(); i++) {
                // tuples[i]->
            }
        }
    }
    return RC::EMPTY; 
}