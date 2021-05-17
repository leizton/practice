无需手动delete, 避免内存泄漏
出现异常时也会执行delete
  example
  ~~~c++
  auto ptr = std::make_shared<Aoo>()
  funcMayThrowException()
  ~~~


--------------------------------------------------------------------------------------------------------------
# make_shared
make_shared 创建的智能指针, 其计数器和对象数据封装在同一个结构体里
计数器和对象在堆内存上是连续的
优点:
  创建和释放只需一次alloc和free, 性能更好
  `shared_ptr<A> p(new A)` 创建时有两次alloc, ⒈ new A; ⒉ 分配计数器空间
缺点:
  调用reset()后对象空间不会及时释放


--------------------------------------------------------------------------------------------------------------
# pointer cast
~~~c++
// 使用时, U 是有实参自动推导出来
shared_ptr<T> static_pointer_cast<T,U>(const shared_ptr<U>& ptr) {
  auto* p = static_cast<T*>(ptr.get())
  return shared_ptr<T>(ptr, p)
}

shared_ptr<T> dynamic_pointer_cast<T,U>(const shared_ptr<U>& ptr) {
  if (auto* p = dynamic_cast<T*>(ptr.get()))
    return shared_ptr<T>(ptr, p)
  else
    return shared_ptr<T>()
}
~~~


--------------------------------------------------------------------------------------------------------------
# enable_shared_from_this<T>
~~~c++
class Coo : public std::enable_shared_from_this<Coo> {};
auto* c = new Coo;
shared_ptr<Coo> p1(c);
shared_ptr<Coo> p2 = c->shared_from_this();
LOG() << p1.use_count() << ", " << p2.use_count(); // 2, 2
~~~
enable_shared_from_this 内部有一个`weak_ptr<T>`, shared_from_this()是通过weak_ptr创建出来
@ref https://zhiqiang.org/coding/std-make-shared-enable-shared-from-this.html#toc-h3-2


--------------------------------------------------------------------------------------------------------------
# src_code
~~~c++
shared_ptr<T> make_shared<typename T, typename... Args>(Args&&... args) {
  return shared_ptr<T>(std::allocator<T>(), std::forward<Args>(args)...)
}

// shared_ptr.h
class shared_ptr<Tp> {
  using T = typename remove_extent<Tp>::type

  T*            _ptr
  shared_count  _refcount  // 由于是共享的, 所以内部存引用计数器的指针

  shared_ptr<Alloc, ...Args>(Alloc& ac, Args&&... args) {
    _ptr(nullptr)
    _refcount(_ptr, ac, args)
  }

  shared_ptr<T>(T* p) {
    _ptr(p)
    _refcount(p)
    __enable_shared_from_this_helper(_refcount, __p, __p)
  }
}

class shared_count {
  Sp_counted_base*  _pi

  // 实际 new 的是 _Sp_counted_ptr_inplace
  shared_count<T, Alloc, ...Args>(T*& ptr, Alloc& ac, Args&&... args) {
    using sp_cnt_type = Sp_counted_ptr_inplace<T, Alloc>
    sp_cnt_type* mem = ac.allocate(1)  // 略去萃取 allocate_type 的过程
    sp_cnt_type* cnt_ptr = new (mem) sp_cnt_type(ac, std::forward<Args>(args)...)
    ptr = cnt_ptr->_impl._storage._M_ptr()
    _pi = cnt_ptr
  }

  shared_count<T>(T* p) {
    _pi = new Sp_counted_ptr<T>(p)
  }
}

class Sp_counted_ptr_inplace<T, Alloc> : Sp_counted_base {
  // 成员变量 _storage 是 T对象 的实际内存空间
  typename std::aligned_storage<sizeof(T), alignof(T)>::type _storage;
}

class Sp_counted_ptr<T> : Sp_counted_base {
  T* _ptr

  Sp_counted_ptr(T* p) {
    _ptr(p)
  }
}

class Sp_counted_base {
  Atomic_word  _use_count = 1
  Atomic_word  _weak_count = 1

  long get_use_count() = atomic_load_n(&_use_count, __ATOMIC_RELAXED)
  void add_ref_copy() = atomic_add_dispatch(&_use_count, 1)
}

// atomic_word.h
typedef int Atomic_word
~~~