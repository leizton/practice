~~~c++

// @header bits/allocator.h
// 空结构体
class allocator<T>: public __allocator_base<T> {
  template <T1>
  class rebind {
    using other = allocator<T1>
  };
}

// @header bits/c++allocator.h
using __allocator_base<T> = new_allocator<T>

// 对 new/delete 封装
class new_allocator<T> {
  T* allocate(size_t n, const void* = 0) {
    if (n > this.max_size()) std::_throw_bad_alloc()
    return static_cast<T*>(::operator new(n * sizeof(T)))
  }

  void deallocate(T* p, size_t) {
    ::operator delete(p)
  }

  size_t max_size() const noexcept {
    return size_t(-1) / sizeof(T)
  }
};