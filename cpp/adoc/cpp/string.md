~~~c++

@header bits/stringfwd.h
using string = basic_string<char, char_traits<char>, allocator<char>>

class basic_string<Char, Traits, Alloc> {
  typedef typename Alloc::(template rebind<char>)::other Raw_bytes_alloc;
    // 等价于 using Raw_bytes_alloc = allocator<char>

  static const size_t npos = static_cast<size_t>(-1)
  Alloc_hider dataplus_

  basic_string() {
    auto* d = S_construct(size_type(), Char(), Alloc())
    dataplus_(d, Alloc())
  }
  basic_string(const basic_string& s) {
    auto* d = s.M_rep()->M_grab(Alloc(s.get_allocator()), s.get_allocator())
    dataplus_(d, Alloc())
  }
  basic_string(const basic_string& s, size_t pos, size_t n) {
    auto* d = S_construct(s.M_data() + pos, std::min(n, s.size() - pos))
    dataplus_(d, Alloc())
  }

  M_data() = dataplus_.p_

  class Alloc_hider : Alloc {
    Char* p_

    Alloc_hider(Char* data, const Alloc& ac) {
      Alloc(ac)
      p_(data)
    }
  };

  class Rep_base {
    size_t length_
    size_t capacity_
    int refcount_
  };

  class Rep : Rep_base {
    static size_type empty_rep_storage_[] // 空串的data

    Char* M_refdata() = reinterpret_cast<Char*>(this + 1)

    Rep* S_create(size_t cap, size_t old_cap, const Alloc& ac) {
      static const size_t page_size = 4096
      static const size_t malloc_header_size = 4 * sizeof(void*)
      if (cap > old_cap) cap = max(cap, old_cap * 2)
      size_t alloc_size = sizeof(Rep) + (cap + 1) * sizeof(Char)
      size_t heap_size = alloc_size + malloc_header_size
      if cap > old_cap && heap_size > page_size
        size_t extra_size = page_size - heap_size % page_size
        if extra_size > 0
          alloc_size += extra_size
          cap += extra_size / sizeof(Char)
      void* place = Raw_bytes_alloc(ac).allocate(alloc_size)
      Rep* rep = new (place) Rep
      rep->capacity_ = cap
      rep->refcount_ = 0
      return rep
    }
  };

  M_copy(Char* dst, Char* src, size_t n) {
    n == 1 ? Traits::assign(*dst, *src) : Traits::copy(dst, src, n)
  }

  S_construct<InputIter>(InputIter begin, InputIter end, const Alloc& ac, input_iterator_tag) {
    // input_iterator_tag 是可读迭代器
  }
}