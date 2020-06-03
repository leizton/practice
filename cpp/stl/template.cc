#include "util/base.h"

template <typename T>
__attribute__((const))
const std::string& getTypeName() {
  static std::string type_name;
  static std::mutex init_mtx_;
  std::lock_guard<std::mutex> lk(init_mtx_);
  if (!type_name.empty()) {
    return type_name;
  }
  std::string name = __PRETTY_FUNCTION__;
  auto idx = name.rfind('[');
  if (idx == std::string::npos) {
    type_name = std::string(typeid(T).name());
    return type_name;
  }
  std::string prefix = "T = ";
  idx = name.find(prefix, idx);
  if (idx == std::string::npos) {
    type_name = std::string(typeid(T).name());
    return type_name;
  }
  idx += prefix.length();
  if (idx >= name.length()) {
    type_name = std::string(typeid(T).name());
    return type_name;
  }
  auto idx1 = name.find(';', idx);
  if (idx1 == std::string::npos) {
    type_name = name.substr(idx, name.length() - idx - 1);
  } else {
    type_name = name.substr(idx, idx1 - idx);
  }
  return type_name;
}

def(get_name_from_template_type) {
  assert_eq("int", getTypeName<int>());
  assert_eq("unsigned int", getTypeName<uint32_t>());
  assert_eq("Aoo", getTypeName<Aoo>());
  assert_eq("void", getTypeName<void>());
  assert_eq("void", getTypeName<void>());
  assert_eq("void", getTypeName<void>());
}

main_run;