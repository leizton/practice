# 条件变量锁外唤醒
锁外唤醒比锁内唤醒少一次线程上下文切换
threadA() {
  lock
  while is_ready == false
    wait  //入等待集合; 释放锁
  unlock
}
threadB() {
  lock
  set_ready
  unlock
  notify  //通知等待集合的元素去加锁检查
}
锁的作用是保护ready变量的线程安全
threadA的锁保证check和入等待集合的原子性, 即只有在is_ready时才wait, 避免check和wait之间插入set
对于锁外唤醒, threadB的锁还有一个作用是保证set和notify的happens-before, set先于notify发生
