```c++

class _Function_base {
  _Any_data        _M_functor
  _Manager_type    _M_manager

  typedef bool (*_Manager_type)(_Any_data&, const _Any_data&, _Manager_operation)

  union _Any_data {
    _Nocopy_types  _M_unused  // 预留?
    char           _M_pod_data[sizeof(_Nocopy_types)]
  }

  union _Nocopy_types {
    void* _M_object;
    const void* _M_const_object;
    void (*_M_function_pointer)();
    void (_Undefined_class::*_M_member_pointer)();
  }
  class _Undefined_class;
}

template <typename _ResultType, typename ..._ArgTypes>
class function<_ResultType(_ArgTypes...)> : _Maybe_unary_or_binary_function

```