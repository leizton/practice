class shared_ptr<T> : __shared_ptr<T> {
}

enum Lock_policy {
  single, mutex, atomic  // 单线程, 互斥锁, 原子锁
}

class __shared_ptr<T, Lock_policy LP> : __shared_ptr_access {
}