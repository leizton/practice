```c++

class allocator<T>: public __gnu_cxx::__allocator_base<T>;

class __allocator_base<T> {
  对 new/delete 封装
  提供 allocate/deallocate 接口的默认实现

  T* allocate(size_t n) = new(n * sizeof(T))
  deallocate(T* p) = delete(p)
};

```