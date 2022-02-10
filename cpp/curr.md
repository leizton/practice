algo tcmalloc
brpc micro_service_arch distribute
recommend tensorflow flink
machine_learning time_series_forecast

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

序列化中间件
- 支持易用且高性能的反射
- 压缩比和压缩性能和protobuf差距不大
- 有自定义的DSL语言, 便于数据管理
- 可以统一服务中的各类数据，如 UserInfo(profile,history), RecoItem, Request/Response

rcu_rw_lock
https://zhuanlan.zhihu.com/p/89439043

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
https://github.com/apache/incubator-brpc/blob/master/README_cn.md

tensorflow
https://www.cnblogs.com/jicanghai/p/9537282.html
https://zhuanlan.zhihu.com/p/25646408

recommend
https://zhuanlan.zhihu.com/p/115690499
https://www.tensorflow.org/recommenders/examples/basic_retrieval?hl=zh-cn

ConcurrentExpireMap

用户多兴趣
https://blog.csdn.net/EdmundYan/article/details/117754084

Tendis 网络线程池
https://www.bookstack.cn/read/tendis-2.1-zh/0e7591d47faad11b.md

asio库: https://zhuanlan.zhihu.com/p/37590580
Tendis把asio从boost剥离出来

facebook-folly中的fbstring使用了boyer_moore算法, 平均查找性能比std::string快30倍

dssm中如何计算embedding向量
https://zhuanlan.zhihu.com/p/37082976

ConcurrentHashMap & java cache (caffine)

mvp
https://juejin.cn/post/6844903448412946445

ps-worker架构

链接原理

向量化搜索
https://ata.alibaba-inc.com/articles/171326?spm=ata.23639746.0.0.3b3c423d3uMFQz

向量化执行引擎
https://ata.alibaba-inc.com/articles/200621?spm=ata.23639746.0.0.3b3c423d3uMFQz

○ lambda https://www.jianshu.com/p/923d11151027
○ any
○ shared_ptr
○ shared_ptr assign 线程不安全分析
○ AtomicSharedPtr
○ vector 内部字段
○ vector iterator 比较
○ function
○ sort cmp https://www.cnblogs.com/cswuyg/p/10155017.html
○ 粒子群算法
○ 贝叶斯优化

tcmalloc
https://zhuanlan.zhihu.com/p/29216091
https://zhuanlan.zhihu.com/p/48957114

并行b树
https://zhuanlan.zhihu.com/b-tree

flink
http://www.zhaoqiansunli.com.cn/view/22115

ontonotes
Transitioned-based
Graphed-based
neural biLSTM

并查集
https://leetcode-cn.com/leetbook/detail/disjoint-set/

规则引擎
https://tech.meituan.com/2017/06/09/maze-framework.html

https://www.zhihu.com/column/c_1288235772122718208

3D模型
https://syncedreview.com/2020/08/10/tour-the-world-from-your-couch-google-nerf-w-delivers-accurate-3d-scene-reconstruction-of-complex-outdoor-environments/

页面置换算法
tiny lfu
https://nan01ab.github.io/2018/04/TinyLFU.html
https://github.com/mandreyel/w-tinylfu
https://www.cnblogs.com/rickiyang/p/11074158.html
https://cloud.tencent.com/developer/article/1520182
http://highscalability.com/blog/2016/1/25/design-of-a-modern-cache.html

http://highscalability.com/all-time-favorites/

向量检索：https://www.pinecone.io/?utm_medium=link&utm_source=high-scalability&utm_campaign=sponsored

图着色

分析代码生成数据流

nginx netty 内存池设计

拓扑排序
树状数组
最小编辑距离

promise https://zhuanlan.zhihu.com/p/58428287

open-source reco-sys with cf lr
EasyRec

eigen
http://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html

动态规划
https://leetcode.com/discuss/general-discussion/458695/dynamic-programming-patterns