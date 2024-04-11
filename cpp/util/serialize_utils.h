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

#define SERIALIZE_UTILS_FIELD_DEFINE(fd, ...) __VA_ARGS__ fd;
#define SERIALIZE_UTILS_BYTE_SIZE(fd, ...) sz += SerializeUtils::ByteSize(fd);
#define SERIALIZE_UTILS_SERIALIZE(fd, ...) offset = SerializeUtils::Serialize(fd, output, offset);
#define SERIALIZE_UTILS_DESERIALIZE(fd, ...) offset = SerializeUtils::Deserialize(fd, input, input_size, offset);

#define SERIALIZE_UTILS_API_IMPL \
  SERIALIZE_UTILS_FIELD_LIST(SERIALIZE_UTILS_FIELD_DEFINE) \
  int ByteSize() const override { \
    int sz = 0; \
    SERIALIZE_UTILS_FIELD_LIST(SERIALIZE_UTILS_BYTE_SIZE) \
    return sz; \
  } \
  int Serialize(uint8_t* output, int offset) const override { \
    SERIALIZE_UTILS_FIELD_LIST(SERIALIZE_UTILS_SERIALIZE) \
    return offset; \
  } \
  int Deserialize(const uint8_t* input, const int input_size, int offset) override { \
    SERIALIZE_UTILS_FIELD_LIST(SERIALIZE_UTILS_DESERIALIZE) \
    return offset; \
  }

class SerializeApi {
public:
  virtual ~SerializeApi() {};

  virtual int ByteSize() const = 0;
  virtual int Serialize(uint8_t* output, int offset) const = 0;
  virtual int Deserialize(const uint8_t* input, const int input_size, int offset) = 0;
};

class SerializeUtils {
public:
  static int SerializeToString(const SerializeApi& obj, std::string& output);
  static int DeserializeFromString(SerializeApi& obj, const std::string& input);

  static int ByteSize(const SerializeApi& obj);
  static int Serialize(const SerializeApi& obj, uint8_t* output, int offset);
  static int Deserialize(SerializeApi& obj, const uint8_t* input, const int input_size, int offset);

  static int ByteSize(bool i);
  static int Serialize(bool i, uint8_t* output, int offset);
  static int Deserialize(bool& i, const uint8_t* input, const int input_size, int offset);

  static int ByteSize(int32_t i);
  static int Serialize(int32_t i, uint8_t* output, int offset);
  static int Deserialize(int32_t& i, const uint8_t* input, const int input_size, int offset);

  static int ByteSize(uint32_t i);
  static int Serialize(uint32_t i, uint8_t* output, int offset);
  static int Deserialize(uint32_t& i, const uint8_t* input, const int input_size, int offset);

  static int ByteSize(int64_t i);
  static int Serialize(int64_t i, uint8_t* output, int offset);
  static int Deserialize(int64_t& i, const uint8_t* input, const int input_size, int offset);

  static int ByteSize(uint64_t i);
  static int Serialize(uint64_t i, uint8_t* output, int offset);
  static int Deserialize(uint64_t& i, const uint8_t* input, const int input_size, int offset);

  static int ByteSize(float d);
  static int Serialize(float d, uint8_t* output, int offset);
  static int Deserialize(float& d, const uint8_t* input, const int input_size, int offset);

  static int ByteSize(double d);
  static int Serialize(double d, uint8_t* output, int offset);
  static int Deserialize(double& d, const uint8_t* input, const int input_size, int offset);

  static int ByteSize(const std::string& s);
  static int Serialize(const std::string& s, uint8_t* output, int offset);
  static int Deserialize(std::string& s, const uint8_t* input, const int input_size, int offset);

  // static int ByteSize(const ::google::protobuf::Message& msg);
  // static int Serialize(const ::google::protobuf::Message& msg, uint8_t* output, int offset);
  // static int Deserialize(::google::protobuf::Message& msg, const uint8_t* input, const int input_size, int offset);

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

  template <class K, class V> static int ByteSize(const std::unordered_map<K, V>& m);
  template <class K, class V> static int Serialize(const std::unordered_map<K, V>& m, uint8_t* output, int offset);
  template <class K, class V> static int Deserialize(std::unordered_map<K, V>& m, const uint8_t* input, const int input_size, int offset);

  template <class K, class V> static int ByteSize(const std::map<K, V>& m);
  template <class K, class V> static int Serialize(const std::map<K, V>& m, uint8_t* output, int offset);
  template <class K, class V> static int Deserialize(std::map<K, V>& m, const uint8_t* input, const int input_size, int offset);

  template <class V> static int ByteSize(const google::dense_hash_map<std::string, V>& m);
  template <class V> static int Serialize(const google::dense_hash_map<std::string, V>& m, uint8_t* output, int offset);
  template <class V> static int Deserialize(google::dense_hash_map<std::string, V>& m, const uint8_t* input, const int input_size, int offset);

  template <class K, class V> static int ByteSize(const std::pair<K, V>& m);
  template <class K, class V> static int Serialize(const std::pair<K, V>& m, uint8_t* output, int offset);
  template <class K, class V> static int Deserialize(std::pair<K, V>& m, const uint8_t* input, const int input_size, int offset);

  template <class T> static int ByteSize(const std::vector<T>& v);
  template <class T> static int Serialize(const std::vector<T>& v, uint8_t* output, int offset);
  template <class T> static int Deserialize(std::vector<T>& v, const uint8_t* input, const int input_size, int offset);

  template <class T> static int ByteSize(const std::unordered_set<T>& v);
  template <class T> static int Serialize(const std::unordered_set<T>& v, uint8_t* output, int offset);
  template <class T> static int Deserialize(std::unordered_set<T>& v, const uint8_t* input, const int input_size, int offset);

  template <class T> static int ByteSize(const std::set<T>& v);
  template <class T> static int Serialize(const std::set<T>& v, uint8_t* output, int offset);
  template <class T> static int Deserialize(std::set<T>& v, const uint8_t* input, const int input_size, int offset);
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
    if (offset < 0) return -1;
  }
  return offset;
}

template <class K, class V>
int SerializeUtils::Deserialize(std::unordered_map<K, V>& m, const uint8_t* input, const int input_size, int offset) {
  m.clear();
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
    if (offset < 0) return -1;
  }
  return offset;
}

template <class K, class V>
int SerializeUtils::Deserialize(std::map<K, V>& m, const uint8_t* input, const int input_size, int offset) {
  m.clear();
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
int SerializeUtils::ByteSize(const google::dense_hash_map<std::string, V>& m) {
  uint32_t len = m.size();
  int sz = ByteSize(len);
  for (const auto& kv : m) {
    sz += ByteSize(kv.first);
    sz += ByteSize(kv.second);
  }
  return sz;
}

template <class V>
int SerializeUtils::Serialize(const google::dense_hash_map<std::string, V>& m, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = m.size();
  offset = Serialize(len, output, offset);
  for (const auto& kv : m) {
    offset = Serialize(kv.first, output, offset);
    offset = Serialize(kv.second, output, offset);
    if (offset < 0) return -1;
  }
  return offset;
}

template <class V>
int SerializeUtils::Deserialize(google::dense_hash_map<std::string, V>& m, const uint8_t* input, const int input_size, int offset) {
  m.set_empty_key("");
  m.clear();
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
    if (offset < 0) return -1;
  }
  return offset;
}

template <class T>
int SerializeUtils::Deserialize(std::vector<T>& v, const uint8_t* input, const int input_size, int offset) {
  v.clear();
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
    if (offset < 0) return -1;
  }
  return offset;
}

template <class T>
int SerializeUtils::Deserialize(std::unordered_set<T>& v, const uint8_t* input, const int input_size, int offset) {
  v.clear();
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
    if (offset < 0) return -1;
  }
  return offset;
}

template <class T>
int SerializeUtils::Deserialize(std::set<T>& v, const uint8_t* input, const int input_size, int offset) {
  v.clear();
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
