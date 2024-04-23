#pragma once

#include <type_traits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <map>

// #include <google/protobuf/message.h>
#include <sparsehash/dense_hash_map>

#include "logger.h"

#define SERIALIZEUTILS_FIELD_DEFINE(fid, fd, ...) __VA_ARGS__ fd;
#define SERIALIZEUTILS_BYTE_SIZE(fid, fd, ...) sz += SerializeUtils::FdByteSize(fid, fd);
#define SERIALIZEUTILS_SERIALIZE(fid, fd, ...) { \
  if (fid <= prev_fid) { \
    LOG_ERROR() << "FdSerialize fid disorder: " << fid << ", " << #fd; \
    return -1; \
  } \
  prev_fid = fid; \
  offset = SerializeUtils::FdSerialize(fid, fd, output, offset); \
  if (offset < 0) { \
    LOG_ERROR() << "FdSerialize fail: " << fid << ", " << #fd << ", " << offset; \
    return -1; \
  } \
}
#define SERIALIZEUTILS_DESERIALIZE(fid, fd, ...) { \
  offset = SerializeUtils::FdDeserialize(fid, #fd, fd, input, input_size, offset); \
  if (offset < 0) { \
    LOG_ERROR() << "FdDeserialize fail: " << fid << ", " << #fd << ", " << input_size << ", " << offset; \
    return -1; \
  } \
}
#define SERIALIZEUTILS_DESERIALIZE_CLEAR(fid, fd, ...) SerializeUtils::DeserializeClear(fd);

#define SERIALIZEUTILS_API_IMPL_INNER \
  int ByteSize_() const { \
    int sz = 8; \
    SERIALIZEUTILS_FIELD_LIST(SERIALIZEUTILS_BYTE_SIZE) \
    serialize_size_ = sz; \
    return sz; \
  } \
  int Serialize_(uint8_t* output, int offset) const { \
    int begin_offset = offset; offset += 8; int prev_fid = 0; \
    SERIALIZEUTILS_FIELD_LIST(SERIALIZEUTILS_SERIALIZE) \
    SerializeUtils::Serialize(begin_offset, output, begin_offset); \
    SerializeUtils::Serialize(offset, output, begin_offset+4); \
    if (offset < 0) { LOG_ERROR() << "Serialize fail offset=" << offset; return -1; } \
    if (offset-begin_offset != serialize_size_) { \
      LOG_ERROR() << "Serialize fail (offset-begin_offset != serialize_size_). " << offset << ", " << begin_offset << ", " << serialize_size_; \
      return -1; \
    } \
    return offset; \
  } \
  int Deserialize_(const uint8_t* input, const int input_size_src, int offset_src) { \
    int offset = 0; SerializeUtils::Deserialize(offset, input, input_size_src, offset_src); \
    if (offset != offset_src) { LOG_ERROR() << "Deserialize fail (offset != offset_src). " << offset << ", " << offset_src; return -1; } \
    offset += 4; \
    int input_size = 0; offset = SerializeUtils::Deserialize(input_size, input, input_size_src, offset); \
    if (input_size > input_size_src) { LOG_ERROR() << "Deserialize fail (input_size > input_size_src)." << input_size << ", " << input_size_src; return -1; } \
    SERIALIZEUTILS_FIELD_LIST(SERIALIZEUTILS_DESERIALIZE) \
    if (offset != input_size) { LOG_ERROR() << "Deserialize fail (offset != input_size)." << offset << ", " << input_size; return -1; } \
    return offset; \
  } \
  void DeserializeClear_() { \
    SERIALIZEUTILS_FIELD_LIST(SERIALIZEUTILS_DESERIALIZE_CLEAR) \
  }
#define SERIALIZEUTILS_API_IMPL_NO_CLEAR \
  SERIALIZEUTILS_FIELD_LIST(SERIALIZEUTILS_FIELD_DEFINE) \
  SERIALIZEUTILS_API_IMPL_INNER \
  int ByteSize() const override { return ByteSize_(); } \
  int Serialize(uint8_t* output, int offset) const override { return Serialize_(output, offset); } \
  int Deserialize(const uint8_t* input, const int input_size, int offset) override { return Deserialize_(input, input_size, offset); }
#define SERIALIZEUTILS_API_IMPL \
  SERIALIZEUTILS_API_IMPL_NO_CLEAR \
  void DeserializeClear() override { DeserializeClear_(); }
#define SERIALIZEUTILS_API_IMPL_WITH_BASE_CLASS(BaseClass) \
  SERIALIZEUTILS_FIELD_LIST(SERIALIZEUTILS_FIELD_DEFINE) \
  SERIALIZEUTILS_API_IMPL_INNER \
  int ByteSize() const override { \
    return BaseClass::ByteSize() + ByteSize_(); \
  } \
  int Serialize(uint8_t* output, int offset) const override { \
    offset = BaseClass::Serialize(output, offset); if (offset < 0) return -1; \
    offset = BaseClass::Serialize(output, offset); \
    if (offset < 0) { LOG_ERROR() << #BaseClass << "::Serialize fail"; return -1; } \
    return Serialize_(output, offset); \
  } \
  int Deserialize(const uint8_t* input, const int input_size, int offset) override { \
    offset = BaseClass::Deserialize(input, input_size, offset); if (offset < 0) return -1; \
    return Deserialize_(input, input_size, offset); \
  } \
  void DeserializeClear() override { \
    BaseClass::DeserializeClear(); \
    DeserializeClear_(); \
  }

class SerializeApi {
public:
  virtual ~SerializeApi() {};

  virtual int ByteSize() const = 0;
  virtual int Serialize(uint8_t* output, int offset) const = 0;
  virtual int Deserialize(const uint8_t* input, const int input_size, int offset) = 0;
  virtual void DeserializeClear() = 0;
  
protected:
  mutable int serialize_size_ = -1;
};

class SerializeUtils {
public:
  static int SerializeToString(const SerializeApi& obj, std::string& output);
  static int DeserializeFromString(SerializeApi& obj, const std::string& input);

  static int ByteSize(const SerializeApi& obj);
  static int Serialize(const SerializeApi& obj, uint8_t* output, int offset);
  static int Deserialize(SerializeApi& obj, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(SerializeApi& obj) { obj.DeserializeClear(); }

  static int ByteSize(bool i);
  static int Serialize(bool i, uint8_t* output, int offset);
  static int Deserialize(bool& i, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(bool& i) { i = false; }

  static int ByteSize(int16_t i);
  static int Serialize(int16_t i, uint8_t* output, int offset);
  static int Deserialize(int16_t& i, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(int16_t& i) { i = 0; }

  static int ByteSize(uint16_t i);
  static int Serialize(uint16_t i, uint8_t* output, int offset);
  static int Deserialize(uint16_t& i, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(uint16_t& i) { i = 0; }

  static int ByteSize(int32_t i);
  static int Serialize(int32_t i, uint8_t* output, int offset);
  static int Deserialize(int32_t& i, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(int32_t& i) { i = 0; }

  static int ByteSize(uint32_t i);
  static int Serialize(uint32_t i, uint8_t* output, int offset);
  static int Deserialize(uint32_t& i, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(uint32_t& i) { i = 0; }

  static int ByteSize(int64_t i);
  static int Serialize(int64_t i, uint8_t* output, int offset);
  static int Deserialize(int64_t& i, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(int64_t& i) { i = 0; }

  static int ByteSize(uint64_t i);
  static int Serialize(uint64_t i, uint8_t* output, int offset);
  static int Deserialize(uint64_t& i, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(uint64_t& i) { i = 0; }

  static int ByteSize(float d);
  static int Serialize(float d, uint8_t* output, int offset);
  static int Deserialize(float& d, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(float& i) { i = 0; }

  static int ByteSize(double d);
  static int Serialize(double d, uint8_t* output, int offset);
  static int Deserialize(double& d, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(double& i) { i = 0; }

  static int ByteSize(const std::string& s);
  static int Serialize(const std::string& s, uint8_t* output, int offset);
  static int Deserialize(std::string& s, const uint8_t* input, const int input_size, int offset);
  static void DeserializeClear(std::string& s) { s.clear(); }

  // static int ByteSize(const ::google::protobuf::Message& msg);
  // static int Serialize(const ::google::protobuf::Message& msg, uint8_t* output, int offset);
  // static int Deserialize(::google::protobuf::Message& msg, const uint8_t* input, const int input_size, int offset);
  // static void DeserializeClear(::google::protobuf::Message& msg) { msg.Clear(); }

  template <typename E, typename = typename std::enable_if<std::is_enum<E>::value, E>::type>
  static int ByteSize(E e) { return 4; }
  template <typename E, typename = typename std::enable_if<std::is_enum<E>::value, E>::type>
  static int Serialize(E e, uint8_t* output, int offset) {
    return Serialize(static_cast<int32_t>(e), output, offset);
  }
  template <typename E, typename = typename std::enable_if<std::is_enum<E>::value, E>::type>
  static int Deserialize(E& e, const uint8_t* input, const int input_size, int offset) {
    return Deserialize(reinterpret_cast<int32_t&>(e), input, input_size, offset);
  }
  template <typename E, typename = typename std::enable_if<std::is_enum<E>::value, E>::type>
  static void DeserializeClear(E& e) { e = static_cast<E>(0); }

  template <class K, class V> static int ByteSize(const std::unordered_map<K, V>& m);
  template <class K, class V> static int Serialize(const std::unordered_map<K, V>& m, uint8_t* output, int offset);
  template <class K, class V> static int Deserialize(std::unordered_map<K, V>& m, const uint8_t* input, const int input_size, int offset);
  template <class K, class V> static void DeserializeClear(std::unordered_map<K, V>& m) { m.clear(); }

  template <class K, class V> static int ByteSize(const std::map<K, V>& m);
  template <class K, class V> static int Serialize(const std::map<K, V>& m, uint8_t* output, int offset);
  template <class K, class V> static int Deserialize(std::map<K, V>& m, const uint8_t* input, const int input_size, int offset);
  template <class K, class V> static void DeserializeClear(std::map<K, V>& m) { m.clear(); }

  template <class V> static int ByteSize(const ::google::dense_hash_map<std::string, V>& m);
  template <class V> static int Serialize(const ::google::dense_hash_map<std::string, V>& m, uint8_t* output, int offset);
  template <class V> static int Deserialize(::google::dense_hash_map<std::string, V>& m, const uint8_t* input, const int input_size, int offset);
  template <class K, class V> static void DeserializeClear(::google::dense_hash_map<K, V>& m) { m.set_empty_key(""); m.clear(); }

  template <class K, class V> static int ByteSize(const std::pair<K, V>& m);
  template <class K, class V> static int Serialize(const std::pair<K, V>& m, uint8_t* output, int offset);
  template <class K, class V> static int Deserialize(std::pair<K, V>& m, const uint8_t* input, const int input_size, int offset);
  template <class K, class V> static void DeserializeClear(std::pair<K, V>& m) { DeserializeClear(m.first); DeserializeClear(m.second); }

  template <class T> static int ByteSize(const std::vector<T>& v);
  template <class T> static int Serialize(const std::vector<T>& v, uint8_t* output, int offset);
  template <class T> static int Deserialize(std::vector<T>& v, const uint8_t* input, const int input_size, int offset);
  template <class T> static void DeserializeClear(std::vector<T>& v) { v.clear(); }

  template <class T> static int ByteSize(const std::unordered_set<T>& v);
  template <class T> static int Serialize(const std::unordered_set<T>& v, uint8_t* output, int offset);
  template <class T> static int Deserialize(std::unordered_set<T>& v, const uint8_t* input, const int input_size, int offset);
  template <class T> static void DeserializeClear(std::unordered_set<T>& v) { v.clear(); }

  template <class T> static int ByteSize(const std::set<T>& v);
  template <class T> static int Serialize(const std::set<T>& v, uint8_t* output, int offset);
  template <class T> static int Deserialize(std::set<T>& v, const uint8_t* input, const int input_size, int offset);
  template <class T> static void DeserializeClear(std::set<T>& v) { v.clear(); }

  template <class T> static int FdByteSize(int fid, const T& v);
  template <class T> static int FdSerialize(int fid, const T& v, uint8_t* output, int offset);
  template <class T> static int FdDeserialize(int fid, const char* fd, T& v, const uint8_t* input, const int input_size, int offset);
};

template <class K, class V>
int SerializeUtils::ByteSize(const std::unordered_map<K, V>& m) {
  uint32_t len = m.size();
  int sz = ByteSize(len);
  for (const auto& kv : m) {
    sz += ByteSize(kv.first);
    sz += ByteSize(kv.second);
  }
  return sz;
}

template <class K, class V>
int SerializeUtils::Serialize(const std::unordered_map<K, V>& m, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = m.size();
  offset = Serialize(len, output, offset);
  for (const auto& kv : m) {
    offset = Serialize(kv.first, output, offset);
    offset = Serialize(kv.second, output, offset);
  }
  return offset;
}

template <class K, class V>
int SerializeUtils::Deserialize(std::unordered_map<K, V>& m, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  uint32_t len = 0u;
  offset = Deserialize(len, input, input_size, offset);
  if (offset < 0) return -1;
  if (len == 0u) return offset;
  for (uint32_t i = 0u; i < len; ++i) {
    std::pair<K, V> pair;
    offset = Deserialize(pair.first, input, input_size, offset);
    offset = Deserialize(pair.second, input, input_size, offset);
    if (offset < 0) return -1;
    m.insert(pair);
  }
  return offset;
}

template <class K, class V>
int SerializeUtils::ByteSize(const std::map<K, V>& m) {
  uint32_t len = m.size();
  int sz = ByteSize(len);
  for (const auto& kv : m) {
    sz += ByteSize(kv.first);
    sz += ByteSize(kv.second);
  }
  return sz;
}

template <class K, class V>
int SerializeUtils::Serialize(const std::map<K, V>& m, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = m.size();
  offset = Serialize(len, output, offset);
  for (const auto& kv : m) {
    offset = Serialize(kv.first, output, offset);
    offset = Serialize(kv.second, output, offset);
  }
  return offset;
}

template <class K, class V>
int SerializeUtils::Deserialize(std::map<K, V>& m, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  uint32_t len = 0u;
  offset = Deserialize(len, input, input_size, offset);
  if (offset < 0) return -1;
  if (len == 0u) return offset;
  for (uint32_t i = 0u; i < len; ++i) {
    std::pair<K, V> pair;
    offset = Deserialize(pair.first, input, input_size, offset);
    offset = Deserialize(pair.second, input, input_size, offset);
    if (offset < 0) return -1;
    m.insert(pair);
  }
  return offset;
}

template <class V>
int SerializeUtils::ByteSize(const ::google::dense_hash_map<std::string, V>& m) {
  uint32_t len = m.size();
  int sz = ByteSize(len);
  for (const auto& kv : m) {
    sz += ByteSize(kv.first);
    sz += ByteSize(kv.second);
  }
  return sz;
}

template <class V>
int SerializeUtils::Serialize(const ::google::dense_hash_map<std::string, V>& m, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = m.size();
  offset = Serialize(len, output, offset);
  for (const auto& kv : m) {
    offset = Serialize(kv.first, output, offset);
    offset = Serialize(kv.second, output, offset);
  }
  return offset;
}

template <class V>
int SerializeUtils::Deserialize(::google::dense_hash_map<std::string, V>& m, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  uint32_t len = 0u;
  offset = Deserialize(len, input, input_size, offset);
  if (offset < 0) return -1;
  if (len == 0u) return offset;
  for (uint32_t i = 0u; i < len; ++i) {
    std::pair<std::string, V> pair;
    offset = Deserialize(pair.first, input, input_size, offset);
    offset = Deserialize(pair.second, input, input_size, offset);
    if (offset < 0) return -1;
    m.insert(pair);
  }
  return offset;
}

template <class K, class V>
int SerializeUtils::ByteSize(const std::pair<K, V>& kv) {
  int sz = ByteSize(kv.first);
  sz += ByteSize(kv.second);
  return sz;
}

template <class K, class V>
int SerializeUtils::Serialize(const std::pair<K, V>& kv, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  offset = Serialize(kv.first, output, offset);
  offset = Serialize(kv.second, output, offset);
  return offset;
}

template <class K, class V>
int SerializeUtils::Deserialize(std::pair<K, V>& kv, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  offset = Deserialize(kv.first, input, input_size, offset);
  offset = Deserialize(kv.second, input, input_size, offset);
  return offset;
}

template <class T>
int SerializeUtils::ByteSize(const std::vector<T>& v) {
  uint32_t len = v.size();
  int sz = ByteSize(len);
  for (const auto& e : v) {
    sz += ByteSize(e);
  }
  return sz;
}

template <class T>
int SerializeUtils::Serialize(const std::vector<T>& v, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = v.size();
  offset = Serialize(len, output, offset);
  for (const auto& e : v) {
    offset = Serialize(e, output, offset);
  }
  return offset;
}

template <class T>
int SerializeUtils::Deserialize(std::vector<T>& v, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  uint32_t len = 0u;
  offset = Deserialize(len, input, input_size, offset);
  if (offset < 0) return -1;
  if (len == 0u) return offset;
  for (uint32_t i = 0u; i < len; ++i) {
    v.emplace_back();
    offset = Deserialize(v.back(), input, input_size, offset);
    if (offset < 0) return -1;
  }
  return offset;
}

template <class T>
int SerializeUtils::ByteSize(const std::unordered_set<T>& v) {
  uint32_t len = v.size();
  int sz = ByteSize(len);
  for (const auto& e : v) {
    sz += ByteSize(e);
  }
  return sz;
}

template <class T>
int SerializeUtils::Serialize(const std::unordered_set<T>& v, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = v.size();
  offset = Serialize(len, output, offset);
  for (const auto& e : v) {
    offset = Serialize(e, output, offset);
  }
  return offset;
}

template <class T>
int SerializeUtils::Deserialize(std::unordered_set<T>& v, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  uint32_t len = 0u;
  offset = Deserialize(len, input, input_size, offset);
  if (offset < 0) return -1;
  if (len == 0u) return offset;
  for (uint32_t i = 0u; i < len; ++i) {
    T e;
    offset = Deserialize(e, input, input_size, offset);
    if (offset < 0) return -1;
    v.insert(e);
  }
  return offset;
}

template <class T>
int SerializeUtils::ByteSize(const std::set<T>& v) {
  uint32_t len = v.size();
  int sz = ByteSize(len);
  for (const auto& e : v) {
    sz += ByteSize(e);
  }
  return sz;
}

template <class T>
int SerializeUtils::Serialize(const std::set<T>& v, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = v.size();
  offset = Serialize(len, output, offset);
  for (const auto& e : v) {
    offset = Serialize(e, output, offset);
  }
  return offset;
}

template <class T>
int SerializeUtils::Deserialize(std::set<T>& v, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  uint32_t len = 0u;
  offset = Deserialize(len, input, input_size, offset);
  if (offset < 0) return -1;
  if (len == 0u) return offset;
  for (uint32_t i = 0u; i < len; ++i) {
    T e;
    offset = Deserialize(e, input, input_size, offset);
    if (offset < 0) return -1;
    v.insert(e);
  }
  return offset;
}

template <class T>
int SerializeUtils::FdByteSize(int fid, const T& v) {
  return 8 + ByteSize(v);
}

template <class T>
int SerializeUtils::FdSerialize(int fid, const T& v, uint8_t* output, int offset) {
  int begin_offset = offset;
  offset = Serialize(fid, output, offset+4);
  offset = Serialize(v, output, offset);
  Serialize(offset, output, begin_offset);
  return offset;
}

template <class T>
int SerializeUtils::FdDeserialize(int fid, const char* fd, T& v, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  if (offset == input_size) {
    LOG_ERROR() << "FdDeserialize lost field: " << fid << ", " << fd;
    return input_size;
  }
  int next_offset = -1;
  while (offset < input_size-8) {
    int curr_offset = offset;
    offset = Deserialize(next_offset, input, input_size, offset);
    if (next_offset > input_size) return -1;
    int parsed_fid = 0;
    offset = Deserialize(parsed_fid, input, input_size, offset);
    if (parsed_fid == fid) {
      offset = Deserialize(v, input, input_size, offset);
      return (offset == next_offset) ? offset : -1;
    }
    if (parsed_fid <= 0 || offset < 0 || next_offset <= offset) return -1;
    if (parsed_fid > fid) {  // fid字段数据缺失
      LOG_ERROR() << "FdDeserialize lost field: " << fid << ", " << fd;
      return curr_offset;
    }
    LOG_ERROR() << "FdDeserialize ignore field: " << fid << ", " << fd;
    offset = next_offset;  // parsed_fid字段已删除
  }
  LOG_ERROR() << "FdDeserialize lost field: " << fid << ", " << fd;
  return input_size;
}
