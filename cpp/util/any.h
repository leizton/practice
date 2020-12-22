#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>

template <typename T>
const std::string& getTypeName();

class Any {
private:
  struct WrapperBase {
    virtual ~WrapperBase() {}
    virtual const std::type_info& type() const = 0;
    virtual const std::string& type_name() const = 0;
    virtual WrapperBase* clone() const = 0;
  };

  template <class V>
  struct Wrapper : public WrapperBase {
    Wrapper(const V& _v) : v(_v) {}
    const std::type_info& type() const { return typeid(V); }
    virtual const std::string& type_name() const { return getTypeName<V>(); }
    WrapperBase* clone() const { return new Wrapper<V>(v); }
    V v;
  };

public:
  template <class V>
  Any(const V& v) : hold_(new Wrapper<V>(v)) {} // 类型V须支持拷贝构造

  Any(const Any& rhs) { deepcopy(rhs); }

  const std::type_info& type() const { return hold_.get()->type(); }

  const std::string& type_name() const { return hold_.get()->type_name(); }

  void swap(Any& rhs) {
    std::swap(hold_, rhs.hold_);
  }

  Any& operator=(const Any& rhs) {
    if (this != &rhs)
      deepcopy(rhs);
    return *this;
  }

private:
  void deepcopy(const Any& rhs) {
    if (rhs.hold_ == nullptr) {
      hold_.reset();
    } else {
      hold_.reset(rhs.hold_.get()->clone());
    }
  }

private:
  std::shared_ptr<WrapperBase> hold_;

  template <class V>
  friend V* any_cast(Any* any);
  template <class V>
  friend const V* any_cast(const Any* any);
  template <class V>
  friend V& any_cast(Any& any);
  template <class V>
  friend const V& any_cast(const Any& any);
};

template <class V>
V* any_cast(Any* any) {
  if (any->type() == typeid(V)) {
    return &(static_cast<Any::Wrapper<V>*>(any->hold_.get())->v);
  } else {
    std::ostringstream ss;
    ss << "any<" << any->type_name() << "> but<" << getTypeName<V>() << ">";
    throw std::runtime_error(ss.str());
  }
}

template <class V>
const V* any_cast(const Any* any) {
  return any_cast<V>(const_cast<Any*>(any));
}

template <class V>
V& any_cast(Any& any) {
  return *any_cast<V>(&any);
}

template <class V>
const V& any_cast(const Any& any) {
  return *any_cast<V>(&const_cast<Any&>(any));
}

template <typename T>
const std::string& getTypeName() {
  static auto build_ = [](const std::string& name) {
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

  static const std::string type_name_(build_(__PRETTY_FUNCTION__));
  return type_name_;
}
