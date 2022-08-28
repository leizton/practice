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

# 性能问题的排查第一工具是perf
先用perf top看一下cpu消耗在哪些函数上
容器内perf卡住可以到容器外看

# tmp
/usr/src/kernels/tools/perf
-g 统计调用栈 call-graph

# realloc 和 malloc
realloc相比malloc省去拷贝

# Akka
配置文件中指定一些节点是种子节点
种子节点启动后开始创建集群, 通过raft选主
普通节点可以申请加入集群或退出集群
数据同步通过gossip协议

# 架构能力
抽象 分治(分层) 复用 迭代
