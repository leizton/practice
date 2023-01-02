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

# 可重入锁实现原理

# protobuf
~~~c++
UserInfo::CopyFrom(const Message& from) {
  if (&from == this) return;  // Tips
  Clear();
  UserInfo::MergeFrom(from) {
    if (&from == this) return;  // Tips
    const UserInfo* source = dynamic_cast<const UserInfo*>(&from);
    if (source == nullptr) {
      ::google::protobuf::internal::ReflectionOps::Merge(from, to=this) {
        Reflection* from_refl, to_refl = from.GetReflection(), to->GetReflection()
        vector<const FieldDescriptor*> fields;
        from_refl->ListFieldsOmitStripped(from, &fields);  // 取出有值的字段(repeated字段非empty)
        for (auto* field : fields) {
          if (not field->is_repeated()) {
            switch (field->cpp_type()) {
              case FieldDescriptor::CPPTYPE_INT32:
                // Reflection::SetInt32 实现代码搜索 void Reflection::Set##TYPENAME
                // 当两个不同类型msg的field匹配时可以相互赋值, 匹配指每第i个field的类型都相同
                to_refl->SetInt32(to, field, value=from_refl->GetInt32(from, field)) {
                  to_refl->SetField<int32_t>(to, field, value)
                    *(MutableRaw<int32_t>(message, field)) = value
                      schema_.GetFieldOffset(field)
                        return offsets_[field->index()]
                }
                break;
              // Int64 UInt32 ...
            }
          } else {
            // ...
          }
        }
      }
    } else {
      MergeFrom(*source);
    }
  }
}
~~~

# 基础算法
排序 Timsort https://github.com/timsort/cpp-TimSort

# realloc 和 malloc
realloc相比malloc省去拷贝

# memcpy 和 memmove
memmove在有重叠区域时是安全的, memcpy不是

# Akka
配置文件中指定一些节点是种子节点
种子节点启动后开始创建集群, 通过raft选主
普通节点可以申请加入集群或退出集群
数据同步通过gossip协议

# folly vector的优化
https://github.com/facebook/folly/blob/main/folly/docs/FBVector.md
1. 增长率从gcc的2倍变成1.5倍
    2倍的缺陷是新的capacity比之前capacity和还大, 即 1+2¹+2²+…+2ⁿ⁻¹ < 2ⁿ
    所以新分配的chunk不用复用之前已分配的chunk
2. 针对relocated对象的拷贝优化
## relocated & non-relocated Object
vector在扩容时先 copy-construct 新对象, 再 deconstruct 旧对象, 是考虑了non-relocated对象
relocated对象指其值不受内存的绝对地址影响, 无论移动到哪里都是一样的, 例如int 可以用memcpy memmove拷贝
non-relocated对象如
struct Eoo {
  char buf[1024];
  char* point_to_this_buf;
  Eoo() point_to_this_buf(buf) {}
};

# perf
先用perf top看一下cpu消耗在哪些函数上
容器内perf卡住可以到容器外看
/usr/src/kernels/tools/perf
-g 统计调用栈 call-graph

# gcc sort 比较函数不是严格弱序(严格小于)导致越界
[测试文件](test/test_sort.cc)

# 《同步》
个体包含振子，个体间能通信(gossip)，通过通信调整自身振子的频率