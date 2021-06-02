~~~c++

// @header bits/stringfwd.h
using string = basic_string<char, char_traits<char>, allocator<char>>

class basic_string<Char, Traits, Alloc> {
  typedef typename Alloc::(template rebind<char>)::other Raw_bytes_alloc;
    // 等价于 using Raw_bytes_alloc = allocator<char>

  static const size_t npos = static_cast<size_t>(-1)

  // @fields
  Alloc_hider dataplus_

  class Alloc_hider : Alloc {
    Char* p_

    Alloc_hider(Char* data, const Alloc& ac) {
      Alloc(ac)
      p_(data)
    }
  };

  basic_string() {
    #if _GLIBCXX_FULLY_DYNAMIC_STRING == 0
      Char* d = S_empty_rep().M_refdata() // 共享空间
    #else
      Char* d = S_construct(0, Char(), Alloc())
    #endif
    dataplus_(d, Alloc())
  }
  basic_string(const basic_string& s) {
    Char* d = s.M_rep()->M_grab(Alloc(s.get_allocator()), s.get_allocator())
    dataplus_(d, Alloc())
  }
  basic_string(const basic_string& s, size_t pos, size_t n) {
    Char* beg = s.M_data() + pos
    Char* end = s.M_data() + min(n, s.size() - pos)
    Char* d = S_construct(beg, end, Alloc())
    dataplus_(d, Alloc())
  }

  Char* M_data() = dataplus_.p_

  Char* S_construct<FwdIter>(FwdIter beg, FwdIter end, const Alloc& ac, forward_iterator_tag) {
    if (beg == end && ac == Alloc()) return S_empty_rep().M_refdata()
    size_t n = (size_t)std::distance(beg, end)
    Rep* rep = S_create(n, 0, ac)
    Char* ret = rep->M_refdata()
    M_copy(ret, beg, n)
    rep->M_set_length_and_sharable(n)
    return ret
  }

  M_copy(Char* dst, Char* src, size_t n) {
    n == 1 ? (*dst = *src) : Traits::copy(dst, src, n)
  }

  class Rep_base {
    // @fields
    size_t length_
    size_t capacity_
    atomic_int refcount_
  };

  class Rep : Rep_base {
    // 空串的data, 静态变量会被初始化成0, 所以 Rep_base::length_ = 0
    static size_type empty_rep_storage_[
      (sizeof(Rep) + sizeof(Char) + sizeof(size_t) - 1) / sizeof(size_t) // cap=0
    ]

    Char* M_refdata() = reinterpret_cast<Char*>(this + 1)

    static Rep& S_empty_rep() = *reinterpret_cast<Rep*>(&empty_rep_storage_)

    static Rep* S_create(size_t cap, size_t old_cap, const Alloc& ac) {
      static const size_t page_size = 4096
      static const size_t malloc_header_size = 4 * sizeof(void*) // malloc的额外开销
      // calc alloc_size
      if (cap > old_cap) cap = max(cap, old_cap * 2)
      size_t alloc_size = sizeof(Rep) + (cap + 1) * sizeof(Char)
      size_t heap_size = alloc_size + malloc_header_size
      if (cap > old_cap && heap_size > page_size)
        size_t extra_size = page_size - heap_size % page_size
        if (extra_size > 0)
          alloc_size += extra_size
          cap += extra_size / sizeof(Char)
      //
      void* place = Raw_bytes_alloc(ac).allocate(alloc_size)
      Rep* rep = new (place) Rep
      rep->capacity_ = cap
      rep->refcount_ = 0
      return rep
    }

    void M_set_length_and_sharable(size_t n) {
      M_set_sharable()
      length_ = n
      M_refdata()[n] = '\0'
    }

    void M_set_sharable() { refcount_ = 0 }
  };
};