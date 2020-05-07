// @mark_code

class shared_ptr<T> : __shared_ptr<T> {
  template<typename... Args>
  using Constructible = typename enable_if<is_constructible<__shared_ptr<T>, Args...>::value>::type;

  // constructor
  shared_ptr() : __shared_ptr<T>() {}
  shared_ptr<Y, typename=Constructible<Y*>>(Y* p) : __shared_ptr<T>(p) {}  // 保证可以用 (Y*) 构造 __shared_ptr<T>
  shared_ptr<Y, Del, typename=Constructible<Y,Del>>(Y* p, Del d) : __shared_ptr<Y>(p, move(d)) {}
  shared_ptr<Y>(const shared_ptr<Y>& r, T* p) : __shared_ptr<T>(r, p) {
    // stores p and shares ownership with r
    // get() == p && use_count() == r.use_count()
  }

  // copy-constructor
  shared_ptr<Y, typename=Constructible<const shared_ptr<Y>&>>(const shared_ptr<Y>& r) : __shared_ptr<Y>(r) {}

  // move-constructor
  shared_ptr<Y, typename=Constructible<shared_ptr<Y>>>(shared_ptr<Y>&& r) : __shared_ptr<Y>(move(r)) {
    // get() == r.get() && use_count() == r.use_count()
  }
}

// xxx_pointer_cast 用于智能指针.  xxx_cast 用于普通类型
static_pointer_cast<T,U>(const shared_ptr<U>& p) {
  return shared_ptr<T>(p, static_cast<T*>(p.get()))
}
dynamic_pointer_cast<T,U>(const shared_ptr<U>& p) {
  using SP = shared_ptr<T>  // 用 using 替代 typedef
  auto* raw = dynamic_cast<T*>(p.get())
  return raw ? SP(p, raw) : SP()
}

enum Lock_policy {
  single, mutex, atomic  // 单线程, 互斥锁, 原子锁
}

class __shared_ptr_access<T, Lock_policy LP, bool = is_array<T>::value, bool = is_void<T>::value> {
  operator*()  T& = *_M_get()
  operator->() T* = _M_get()
  _M_get()     T* = (__shared_ptr<T,LP>*(this))->get()
}

class __shared_ptr_access<T, Lock_policy LP, true, false> {
  // c++17开始shared_ptr支持[]
  // _M_get 得到数组首地址
  // 对于64位, ptrdiff_t是int64_t; 对于32位, 是int32_t
  // ptrdiff_t可以是负数
  operator[](ptrdiff_t i) T& = _M_get()[i]
}

class __shared_ptr<T, Lock_policy LP> : __shared_ptr_access<T, LP> {
}