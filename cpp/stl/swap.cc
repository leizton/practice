#include "util/base.h"

struct Obj {
  int v;

  Obj() = delete;

  Obj(int _v)
      : v(_v) {}

  Obj(const Obj& rhs)
      : v(rhs.v) { println("copy_con:", rhs.v); }
  Obj(const Obj&& rhs)
      : v(rhs.v) { println("move_copy_con:", rhs.v); }

  Obj& operator=(const Obj& rhs) {
    println("assign:", v, "=", rhs.v);
    v = rhs.v;
    return *this;
  }

  Obj& operator=(const Obj&& rhs) {
    println("move_assign:", v, "=", rhs.v);
    v = rhs.v;
    return *this;
  }

  friend ostream& operator<<(ostream& os, const Obj& o);
};

ostream& operator<<(ostream& os, const Obj& o) {
  os << o.v;
  return os;
}

#if comment(swap implement)

template <class T>
void swap(T& x, T& y) {
  T t(std::move(x)); // call move_copy_con
  x = std::move(y);  // call move_assign
  y = std::move(t);  // call move_assign
}

#endif

def(swap) {
  Obj o1(1), o2(2);
  std::swap(o1, o2);
}

#if commend(vector::swap implement)

void vector<T>::swap(vector<T>& rhs) {
  this->_M_impl._M_swap_data(rhs._M_impl);
}

_M_impl里存了三个指针:  _M_start  _M_finish  _M_end_of_storage
仅swap三个指针

#endif

def(vector_swap) {
  vector<Obj> v1, v2;
  v1.reserve(8);
  v2.reserve(8);
  v1.emplace_back(1);
  v1.emplace_back(2);
  v2.emplace_back(11);
  v2.emplace_back(12);
  v2.emplace_back(13);

  println("before swap:", v1, v2);

  v1.swap(v2); // 没有调用拷贝构造和赋值

  println("after swap:", v1, v2);
}

main_run;