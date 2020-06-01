// @mark_code

class shared_ptr<T> : SharedPtr<T> {  // shared_ptr.h
  template<typename... Args>
  using Constructible = typename enable_if<is_constructible<SharedPtr<T>, Args...>::value>::type;

  // constructor ==============================================================
  shared_ptr() {
    SharedPtr<T>()
  }
  shared_ptr<Y, typename=Constructible<Y*>>(Y* p) {
    // 保证可以用 (Y*) 构造 SharedPtr<T>
    // 可能 T是void, Y是某个类
    SharedPtr<T>(p)
  }
  shared_ptr<Y, Del, typename=Constructible<Y,Del>>(Y* p, Del d) {
    SharedPtr<Y>(p, move(d))
  }

  shared_ptr<Y>(const shared_ptr<Y>& r, T* p) {
    // stores p and shares ownership with r
    // 最终 get() == p && use_count() == r.use_count()
    SharedPtr<T>(r, p)
  }

  // copy-constructor =========================================================
  shared_ptr<Y, typename=Constructible<const shared_ptr<Y>&>>(const shared_ptr<Y>& r) {
    SharedPtr<Y>(r)
  }

  // move-constructor =========================================================
  shared_ptr<Y, typename=Constructible<shared_ptr<Y>>>(shared_ptr<Y>&& r) {
    // 最终 get() == r.get() && use_count() == r.use_count()
    SharedPtr<Y>(move(r))
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

enum _Lock_policy {
  single, mutex, atomic  // 单线程, 互斥锁, 原子锁
}

class SharedPtrAccess<T, bool = is_array<T>::value, bool = is_void<T>::value> {
  // shared_ptr_base.h
  // __shared_ptr_access

  // 重载指针操作符
  operator*()  T& = *_M_get()
  operator->() T* = _M_get()
  _M_get()     T* = (SharedPtr<T>*(this))->get()
}

class SharedPtrAccess<T, true, false> {
  // c++17开始shared_ptr支持[]
  // _M_get 得到数组首地址
  // 对于64位, ptrdiff_t是int64_t; 对于32位, 是int32_t
  // ptrdiff_t可以是负数
  operator[](ptrdiff_t i) T& = _M_get()[i]
}

class SharedPtr<T> : SharedPtrAccess<T> {
  // shared_ptr_base.h
  // @class __shared_ptr

  template <class Y>
  using SafeConv = typename enable_if<__sp_is_constructible<T,Y>::value>::type;

  using element_type = typename remove_extent<T>::type;

  fields() {
    ptr_        element_type*    // _M_ptr
    ref_cnt_    SharedCount      // _M_refcount
  }

  // constructor ==============================================================
  SharedPtr() {
    ptr_(nullptr)
    ref_cnt_()
  }
  SharedPtr(nullptr) {
    SharedPtr()
  }
  SharedPtr<Y, typename=SafeConv<Y>>(Y* p) {
    // 检查 Y* 可以转成 T*
    // 类型 Y 保留到了 `SharedCount ref_cnt_` 的 `SpCountedPtr pi_`
    ptr_(p)
    ref_cnt_(p, typename is_array<_Tp>::type())
  }
  SharedPtr<Y, Del, typename=SafeConv<Y>>(Y* p, Del d) {
    ptr_(p)
    ref_cnt_(p, move(d))
  }

  // assign ===================================================================
  operator=(SharedPtr&& r) SharedPtr& {
    SharedPtr(move(r)).swap(*this)
    return *this
  }
  operator=<Y>(const SharedPtr<Y>& r) Assignable<Y> {
    SharedPtr(move(r)).swap(*this)
  }

  // method ===================================================================
  reset() {
    SharedPtr().swap(*this);
  }
  reset<Y>(Y* p) SaveConv<Y> {
    if (p == nullptr || p != ptr_) {
      SharedPtr(p).swap(*this);
    }
  }

  get() element_type* {
    return ptr_
  }

  swap(SharedPtr<T>& sp) {
    std::swap(ptr_, sp.ptr_)
    ref_cnt_.swap(sp.ref_cnt_)
  }
}

class SharedCount {
  // shared_ptr_base.h
  // @class __shared_count

  fields() {
    pi_    SpCountedBase*    // pointer with type_info
  }

  // constructor ==============================================================
  SharedCount() {
    pi_(nullptr)
  }
  SharedCount<Ptr>(Ptr p) {
    pi_ = new SpCountedPtr<Ptr>(p)
  }
  SharedCount<Ptr, Del>(Ptr p, Del d) {
    pi_ = new SpCountedDeleter<Ptr, Del>(p, move(d))
  }
  SharedCount<Ptr>(Ptr p, false_type/* is_array */) {
    SharedCount(p)
  }
  SharedCount<Ptr>(Ptr p, true_type/* is_array */) {
    struct SpArrayDelete {
      template <class Y>
      void operator()(Y* p) const { delete[] p; }
    };
    SharedCount(p, SpArrayDelete{})
  }

  // de-constructor ===========================================================
  ~SharedCount() {
    if pi_, pi_->_M_release()
  }

  // copy-constructor =========================================================
  SharedCount(SharedCount& r) {
    pi_(r.pi_)
    if pi_, pi_->_M_add_ref_copy()
  }

  // method ===================================================================
  swap(SharedCount& r) {
    SpCountedBase* tmp = r.pi_
    r.pi_ = pi_
    pi_ = tmp
  }
}

class SpCountedBase : _Mutex_base {
  // shared_ptr_base.h
  // @class _Sp_counted_base

  fields() {
    use_cnt_     int
    weak_cnt_    int
  }

  // constructor ==============================================================
  SpCountedBase() {
    use_cnt_(1)
    weak_cnt_(1)
  }

  // method ===================================================================
  _M_destroy() virtual = delete this
  _M_dispose() virtual = 0

  _M_add_ref_copy() {
    atomic_add_dispatch(&use_cnt_, 1)
  }

  _M_release() {
    if exchange_and_add_dispatch(&use_cnt_, -1) == 1
      _M_dispose()
      atomic_thread_fence(__ATOMIC_ACQ_REL)  // a memory barrier between dispose and destroy
      if exchange_and_add_dispatch(&weak_cnt_, -1) == 1
        _M_destroy()
  }
}

class SpCountedPtr<Ptr> : SpCountedBase {
  // shared_ptr_base.h
  // @class _Sp_counted_ptr

  fields() {
    ptr_    Ptr
  }

  // constructor ==============================================================
  SpCountedPtr(Ptr p) {
    ptr_(p)
  }

  // method ===================================================================
  _M_dispose() = delete ptr_
}

class SpCountedDeleter<Ptr> : SpCountedBase {
  // shared_ptr_base.h
  // @class _Sp_counted_deleter

  _M_dispose() = _M_impl._M_del()(_M_impl._M_ptr)
}