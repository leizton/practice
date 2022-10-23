#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>

#include "template_utils.h"

class Any {
private:
  // 基类的作用是: 类型擦除
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
