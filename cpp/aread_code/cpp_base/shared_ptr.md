无需手动delete, 避免内存泄漏
出现异常时也会执行delete
  example
  ```c++
  auto ptr = std::make_shared<Aoo>()
  funcMayThrowException()
  ```


# make_shared
make_shared 创建的智能指针, 其计数器和对象数据封装在同一个结构体里
由于在内存上是在同一个地方, 所以调用reset()后对象空间并没有释放


# src_code

https://zhiqiang.org/coding/std-make-shared-enable-shared-from-this.html


```c++
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
}

class shared_count {
  _Sp_counted_base*  _pi

  // 实际 new 的是 _Sp_counted_ptr_inplace
  shared_count<T, Alloc, ...Args>(T*& ptr, Alloc& ac, Args&&... args) {
    using sp_cnt_type = _Sp_counted_ptr_inplace<_Tp, _Alloc>
    sp_cnt_type* mem = ac.allocate(1)  // 略去萃取 allocate_type 的过程
    sp_cnt_type* cnt_ptr = new (mem) sp_cnt_type(ac, std::forward<Args>(args)...)
    _pi = cnt_ptr
    ptr = cnt_ptr->_impl._storage._M_ptr()
  }
}

class _Sp_counted_ptr_inplace<T, Alloc> : _Sp_counted_base {
  // 成员变量 _storage 是 T对象 的实际内存空间
  typename std::aligned_storage<sizeof(T), alignof(T)>::type _storage;
}

class _Sp_counted_base {
  _Atomic_word  _use_count = 1
  _Atomic_word  _weak_count = 1

  long _get_use_count() = __atomic_load_n(&_use_count, __ATOMIC_RELAXED)
  void _add_ref_copy() = __atomic_add_dispatch(&_use_count, 1)
}

// atomic_word.h
typedef int _Atomic_word
```


# pointer cast
```c++
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
```