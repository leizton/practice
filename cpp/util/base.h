#pragma once

#include "util/cpp_lib.h"
#include "util/print.h"
#include "util/utils.h"
#include "util/too.h"
#include "util/unit_test.h"


/**
 * time
 */
inline uint64_t nowSec() {
  return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline uint64_t nowMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline uint64_t nowUs() {
  return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline std::chrono::duration<int, std::ratio<1LL, 1LL>> buildDurationSec(int sec) {
  return std::chrono::duration<int, std::ratio<1LL, 1LL>>(sec);
}
inline std::chrono::duration<int, std::milli> buildDurationMs(int ms) {
  return std::chrono::duration<int, std::milli>(ms);
}
inline std::chrono::duration<int, std::micro> buildDurationUs(int us) {
  return std::chrono::duration<int, std::micro>(us);
}
inline void sleepSec(int sec) {
  std::this_thread::sleep_for(buildDurationSec(sec));
}
inline void sleepMs(int ms) {
  std::this_thread::sleep_for(buildDurationMs(ms));
}
inline void sleepUs(int us) {
  std::this_thread::sleep_for(buildDurationUs(us));
}


/**
 * Any
 */
class Any {
private:
  struct WrapperBase {
    virtual ~WrapperBase() {}
    virtual const std::type_info& type() const = 0;
    virtual WrapperBase* clone() const = 0;
  };

  template<class V>
  struct Wrapper : public WrapperBase {
    Wrapper(const V& _v) : v(_v) {}
    const std::type_info& type() const { return typeid(V); }
    WrapperBase* clone() const { return new Wrapper<V>(v); }
    V v;
  };

public:
  template<class V> Any(const V& v) : hold_(new Wrapper<V>(v)) {}

  Any(const Any& rhs) { deepcopy(rhs); }

  const std::type_info& type() const { return hold_.get()->type(); }

  void swap(Any& rhs) {
    std::swap(hold_, rhs.hold_);
  }

  Any& operator=(const Any& rhs) {
    if (this != &rhs) deepcopy(rhs);
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

  template<class V> friend V*       any_cast(Any* any);
  template<class V> friend const V* any_cast(const Any* any);
  template<class V> friend V&       any_cast(Any& any);
  template<class V> friend const V& any_cast(const Any& any);
};

template<class V>
V* any_cast(Any* any) {
  if (any->type() == typeid(V)) {
    return &(static_cast<Any::Wrapper<V>*>(any->hold_.get())->v);
  } else {
    ostringstream ss;
    ss << "any<" << any->type().name() << "> but<" << typeid(V).name() << ">";
    throw std::runtime_error(ss.str());
  }
}

template<class V>
const V* any_cast(const Any* any) {
  return any_cast<V>(const_cast<Any*>(any));
}

template<class V>
V& any_cast(Any& any) {
  return *any_cast<V>(&any);
}

template<class V>
const V& any_cast(const Any& any) {
  return *any_cast<V>(&const_cast<Any&>(any));
}
