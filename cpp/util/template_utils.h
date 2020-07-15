#include <string>
#include <string_view>

template <typename T>
const std::string& getTypeName() {
  static auto build_ = [](const std::string_view& name) {
    auto idx = name.rfind('[');
    if (idx == std::string::npos) {
      return std::string(typeid(T).name());
    }
    const char* prefix = "T = ";
    const size_t prefix_len = ::strlen(prefix);
    idx = name.find(prefix, idx);
    if (idx == std::string::npos) {
      return std::string(typeid(T).name());
    }
    idx += prefix_len;
    if (idx >= name.length()) {
      return std::string(typeid(T).name());
    }
    auto idx1 = name.find(';', idx);
    if (idx1 == std::string::npos) {
      return std::string(name.substr(idx, name.length() - idx - 1));
    } else {
      return std::string(name.substr(idx, idx1 - idx));
    }
  };

  static std::string type_name_;
  static std::mutex init_mtx_;
  static std::atomic<bool> inited_(false);

  if (!inited_) {
    std::lock_guard<std::mutex> lk(init_mtx_);
    if (!inited_) {
      type_name_ = build_(__PRETTY_FUNCTION__);
      inited_ = true;
    }
  }
  return type_name_;
}
