class shared_ptr<_Tp> : __shared_ptr<_Tp> {
}

enum _Lock_policy {
  _S_single, _S_mutex, _S_atomic  // 单线程, 互斥锁, 原子锁
}

class __shared_ptr<_Tp, _Lock_policy _Lp> : __shared_ptr_access {
}