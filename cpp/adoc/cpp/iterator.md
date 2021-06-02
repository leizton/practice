~~~c++

// @header bits/stl_iterator_base_types.h
struct input_iterator_tag  {};  // 只读, 只++
struct output_iterator_tag {};  // 只写, 只++
struct forward_iterator_tag : public input_iterator_tag {};  // 可读可写, 只++
struct bidirectional_iterator_tag : public forward_iterator_tag {};  // 可 ++ --
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

前置自增   ++it  返回类型:Iter&
后置自增   it++  返回类型:Iter   等价于 { Iter tmp_it = it; ++it; return tmp_it;  }
后置自增  *it++  返回类型:T      等价于 {  T tmp_val = *it; ++it; return tmp_val; }