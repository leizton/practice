algo recommend tensorflow brpc tcmalloc

linux多线程编程，https://www.codenong.com/cs105103411/

linux应用程序性能调试工具，https://linuxtools-rst.readthedocs.io/zh_CN/latest/tool/pstack.html

推荐系统算法实践，了解召回算法、排序算法
https://zhuanlan.zhihu.com/p/266755203

Disruptor，https://tech.meituan.com/2016/11/18/disruptor.html

mutex & lock, http://hengyunabc.github.io/cpp11-mutex-lock-condition/

posix::ucontext用户级线程实现原理分析, https://segmentfault.com/p/1210000009166339/read

缺页中断

lucene: http://www.nosqlnotes.com/technotes/searchengine/lucene-invertedindex/

lucene FST: https://www.shenyanchao.cn/blog/2018/12/04/lucene-fst/

函数调用树，非局部变量
查找函数调用

gcc 异常回溯

分布式 unique_id

shared_ptr assign 线程不安全分析

AtomicSharedPtr

string vector list AND-SO-ON 内部字段

vector iterator 比较

序列化中间件
- 支持易用且高性能的反射
- 压缩比和压缩性能和protobuf差距不大
- 有自定义的DSL语言, 便于数据管理
- 可以统一服务中的各类数据，如 UserInfo(profile,history), RecoItem, Request/Response

epoll

rcu_rw_lock
https://zhuanlan.zhihu.com/p/89439043

mvp
https://juejin.cn/post/6844903448412946445

ELF
![01](./adoc/base/ELF文件格式.pdf)

MQ
rocketmq -> kafka, 存储模型 大量topic
mq -> kafka, 消费模型 P/C无缝扩缩容
有序消费, 有用 难实现

多样性
https://zhuanlan.zhihu.com/p/264652162
https://zhuanlan.zhihu.com/p/95607668, dpp

brpc
https://zhuanlan.zhihu.com/p/120493593
https://github.com/apache/incubator-brpc/blob/master/docs/cn/atomic_instructions.md
https://github.com/apache/incubator-brpc/blob/master/docs/cn/bvar.md

tensorflow
https://www.cnblogs.com/jicanghai/p/9537282.html
https://zhuanlan.zhihu.com/p/25646408

recommend
https://zhuanlan.zhihu.com/p/115690499
https://www.tensorflow.org/recommenders/examples/basic_retrieval?hl=zh-cn

ConcurrentExpireMap

用户多兴趣
https://blog.csdn.net/EdmundYan/article/details/117754084

static_cast vs reinterpret_cast

限流: 漏桶、令牌桶
https://cloud.tencent.com/developer/article/1408819