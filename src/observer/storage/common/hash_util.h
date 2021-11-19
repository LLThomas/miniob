
#pragma once

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

#include "sql/executor/value.h"
#include "sql/parser/parse.h"

using hash_t = size_t;

class HashUtil {
 private:
  static const hash_t PRIME_FACTOR = 10000019;

 public:
  static inline hash_t HashBytes(const char *bytes, size_t length) {
    // https://github.com/greenplum-db/gpos/blob/b53c1acd6285de94044ff91fbee91589543feba1/libgpos/src/utils.cpp#L126
    hash_t hash = length;
    for (size_t i = 0; i < length; ++i) {
      hash = ((hash << 5) ^ (hash >> 27)) ^ bytes[i];
    }
    return hash;
  }

  static inline hash_t CombineHashes(hash_t l, hash_t r) {
    hash_t both[2];
    both[0] = l;
    both[1] = r;
    return HashBytes(reinterpret_cast<char *>(both), sizeof(hash_t) * 2);
  }

  static inline hash_t SumHashes(hash_t l, hash_t r) {
    return (l % PRIME_FACTOR + r % PRIME_FACTOR) % PRIME_FACTOR;
  }

  template <typename T>
  static inline hash_t Hash(const T *ptr) {
    return HashBytes(reinterpret_cast<const char *>(ptr), sizeof(T));
  }

  template <typename T>
  static inline hash_t HashPtr(const T *ptr) {
    return HashBytes(reinterpret_cast<const char *>(&ptr), sizeof(void *));
  }

  /** @return the hash of the value */
  static inline hash_t HashValue(const TupleValue *val) {
    switch (val->get_type()) {
      case AttrType::INTS: {
        auto raw = static_cast<int64_t>(((IntValue *)val)->get_value());
        return Hash<int64_t>(&raw);
      }
      case AttrType::FLOATS: {
        auto raw = static_cast<int64_t>(((FloatValue *)val)->get_value());
        return Hash<int64_t>(&raw);
      }
      case AttrType::DATES: {
        auto raw = static_cast<int64_t>(((DateValue *)val)->get_value());
        return Hash<int64_t>(&raw);
      }
      case AttrType::CHARS: {
        std::string str = ((StringValue *)val)->get_value();
        auto raw = str.c_str();
        auto len = str.length();
        return HashBytes(raw, len);
      }
        //   case AttrType::DATES : {
        //     auto raw = static_cast<int64_t>(val->GetAs<int16_t>());
        //     return Hash<int64_t>(&raw);
        //   }
        //   case TypeId::INTEGER: {
        //     auto raw = static_cast<int64_t>(val->GetAs<int32_t>());
        //     return Hash<int64_t>(&raw);
        //   }
        //   case TypeId::BIGINT: {
        //     auto raw = static_cast<int64_t>(val->GetAs<int64_t>());
        //     return Hash<int64_t>(&raw);
        //   }
        //   case TypeId::BOOLEAN: {
        //     auto raw = val->GetAs<bool>();
        //     return Hash<bool>(&raw);
        //   }
        //   case TypeId::DECIMAL: {
        //     auto raw = val->GetAs<double>();
        //     return Hash<double>(&raw);
        //   }
        //   case TypeId::VARCHAR: {
        //     auto raw = val->GetData();
        //     auto len = val->GetLength();
        //     return HashBytes(raw, len);
        //   }
        //   case TypeId::TIMESTAMP: {
        //     auto raw = val->GetAs<uint64_t>();
        //     return Hash<uint64_t>(&raw);
        //   }
      default: {
        assert(false && "Unsupported type.");
      }
    }
  }
};
