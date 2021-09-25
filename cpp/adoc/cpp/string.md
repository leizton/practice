~~~c++

// @header bits/stringfwd.h
using string = basic_string<char, char_traits<char>, allocator<char>>

class basic_string<_CharT, _Traits, _Alloc> {
  // 32 bytes
  fields() {
    _Alloc_hider _M_dataplus; // 8 bytes
    size_t _M_string_length;  // 8 bytes

    // 16 bytes
    static int _S_local_capacity = 15 / sizeof(_CharT);
    union {
      char _M_local_buf[_S_local_capacity+1]; // 本地可存15个，减少alloc次数
      size_t _M_allocated_capacity;
    };
  }

  class _Alloc_hider : allocator_type {
    fields() {
      _CharT* _M_p; // the actual data
    }
  }

  _M_data() (_CharT*)& {
    return _M_dataplus._M_p;
  }

  _M_dispose() {
    if (_M_data != _M_local_buf)
      _Alloc::deallocate(_M_data, _M_allocated_capacity + 1);
  }

  basic_string() {
    _M_data = _M_local_buf;
    _M_string_length = 0;
    _M_data[_M_string_length] = 0;
  }

  basic_string(const basic_string& s) {
    _M_data = _M_local_buf;
    _M_construct(s._M_p, s._M_p + s.length());
  }

  // @ref <bits/basic_string.tcc>
  _M_construct(_InIterator beg, _InIterator end, std::input_iterator_tag) {
    // 先用本地空间
    // len：实际字符个数. cap：可存的字符总量，不包括\0
    size_t len = 0, cap = _S_local_capacity;
    while (beg != end && len < cap) {
      _M_data[len++] = *beg;
      ++beg;
    }
    //
    try {
      while (beg != end) {
        if (len == cap) { // 占满了
          cap = len + 1;
          // 至少分配old_cap*2，分配后大小输出到new_cap. 实际alloc大小是(new_cap+1)，最后一个是\0
          auto* new_data = _M_create(old_cap=len, &new_cap=cap);
          _S_copy(new_data, _M_data, len);
          _M_dispose();
          _M_data = new_data;
          _M_allocated_capacity = cap;
        }
        _M_data[len++] = *beg;
        ++beg;
      }
    } catch (...) {
      _M_dispose();
      throw; // throw again
    }
    //
    _M_string_length = len;
    _M_data[_M_string_length] = 0;
  }
};