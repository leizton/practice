~~~c++

@header bits/stl_iterator_base_types.h
struct input_iterator_tag  {};  // 只读迭代器
struct output_iterator_tag {};  // 只写迭代器
struct forward_iterator_tag : public input_iterator_tag {};  // 只读迭代器
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};