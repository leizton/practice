无需手动delete, 避免内存泄漏
出现异常时也会执行delete
  example
  ```c++
  auto ptr = std::make_shared<Aoo>()
  funcMayThrowException()
  ```


# make_shared
make_shared 创建的智能指针, 其计数器和对象数据封装在同一个结构体里
因此在内存上是在同一个地方, 调用reset()后对象空间并没有释放


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
  shared_count  _refcount

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
  Impl  _impl  // 计数器和对象数据在一起

  class Impl {
    __gnu_cxx::__aligned_buffer<T>  _storage
  }
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