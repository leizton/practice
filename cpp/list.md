target: work stock ai

glog log4j
mq
workflow
tcmalloc
  https://zhuanlan.zhihu.com/p/29216091
  https://zhuanlan.zhihu.com/p/48957114
faiss

linux环境编程：SIMD 并发编程 协程 性能调试
  多线程编程 https://www.codenong.com/cs105103411/
  性能调试 https://linuxtools-rst.readthedocs.io/zh_CN/latest/tool/pstack.html

Disruptor，https://tech.meituan.com/2016/11/18/disruptor.html

mutex & lock, http://hengyunabc.github.io/cpp11-mutex-lock-condition/

posix::ucontext用户级线程实现原理分析, https://segmentfault.com/p/1210000009166339/read

缺页中断

lucene: http://www.nosqlnotes.com/technotes/searchengine/lucene-invertedindex/

lucene FST: https://www.shenyanchao.cn/blog/2018/12/04/lucene-fst/

函数调用树，非局部变量
查找函数调用

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
有序消费

用户多兴趣
https://blog.csdn.net/EdmundYan/article/details/117754084

Tendis 网络线程池
https://www.bookstack.cn/read/tendis-2.1-zh/0e7591d47faad11b.md

asio库: https://zhuanlan.zhihu.com/p/37590580
Tendis把asio从boost剥离出来

facebook-folly中的fbstring使用了boyer_moore算法, 平均查找性能比std::string快30倍

dssm中如何计算embedding向量
https://zhuanlan.zhihu.com/p/37082976

mvp
https://juejin.cn/post/6844903448412946445

ps-worker架构

链接原理

向量化搜索
https://ata.alibaba-inc.com/articles/171326?spm=ata.23639746.0.0.3b3c423d3uMFQz

向量化执行引擎
https://ata.alibaba-inc.com/articles/200621?spm=ata.23639746.0.0.3b3c423d3uMFQz

并行b树
https://zhuanlan.zhihu.com/b-tree

NLP语料库
ontonotes
Transitioned-based
Graphed-based
neural biLSTM

拓扑排序
树状数组

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

promise https://zhuanlan.zhihu.com/p/58428287

open-source reco-sys with cf lr
EasyRec

eigen
http://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html

动态规划
https://leetcode.com/discuss/general-discussion/458695/dynamic-programming-patterns

deepmatch
https://github.com/shenweichen/DeepMatch

目标检测和跟踪 base工作->分割
句法结构分析 base工作->分词
形态分析，简笔画风格，漫画风格
点云拟合
能源
sk置信度控盘，在不同置信度时agent和broker的行为不同，挖掘出其行为原理

分类模型：youtube-dnn deepmatch 输入层包括user_fea和行为数据
预估模型：输入层包括user和item的feature、行为数据

注意力机制

mimalloc

flat buffer

https://yaoguais.github.io/article/jemalloc/structures.html

LIRS cache 缓存淘汰算法
https://cloud.tencent.com/developer/article/1005742

https://zhuanlan.zhihu.com/p/525985268

https://www.oreilly.com/library/view/power-and-performance/9780128007266/B9780128007266000082.xhtml#s0010

tensorflow recommendation
https://www.tensorflow.org/lite/examples/recommendation/overview

w2v
https://www.tensorflow.org/tutorials/text/word2vec

https://github.com/NicolasHug/Surprise
https://github.com/practical-recommender-systems/moviegeek
https://github.com/tensorflow/recommenders
https://github.com/alibaba/Alink
https://github.com/gorse-io/gorse
https://github.com/RUCAIBox/RecBole

SVDFeature
https://www.jmlr.org/papers/v13/chen12a.html

ann邻近搜索
https://zhuanlan.zhihu.com/p/266589272
https://github.com/spotify/annoy

CLucene
http://clucene.sourceforge.net/

pid kalman

New Tcmalloc
abseil tips: https://abseil.io/tips/1

NeRF

informer

《大规模推荐系统实战》
practical recommender systems

tensorflow

auVPN

national stats
https://data.stats.gov.cn/index.htm
https://cloud.tencent.com/developer/article/1897244

std::bind

https://github.com/tstarling/thread-safe-lru/wiki/Benchmarks

g++ -std=c++11 main.cc -fsanitize=address -fno-omit-frame-pointer -g

https://github.com/miloyip/itoa-benchmark

transform https://ke.qq.com/course/4964012#

log library
https://binlog.org/UserGuide.html, https://github.com/morganstanley/binlog
https://github.com/PlatformLab/NanoLog
g3log: crash safe
日志性能优化点
  - 调用处不进入内核态
  - 无锁 (线程安全的无锁队列)
  - 不等IO操作 (buffer 后台线程flush)
  - 多个日志行批量IO提高吞吐量 (batch)
  - 调用处不完成format成string

分布式系统设计模式
https://martinfowler.com/articles/patterns-of-distributed-systems/#WhatThisIsAbout

transformer
https://github.com/hellotransformers/Natural_Language_Processing_with_Transformers

单词: 同形(编辑距离小)、近义、反义、相关(出现在同一个上下文)

https://github.com/neo4j-contrib/neo4j-graph-algorithms

what DL has learned. by OCR and morphology

c++ 性能瓶颈
  map hash_map concurrent_hash_map lru
  string-find/compare map<string,val> hash_map<string,val>

proto和arrow相比痛点：
1. 新增字段、修改字段类型需要上下游同步改pb文件费时且容易因前后兼容出错
2. 序列化后的二进制数据不利于流处理分析(OLAP)
3. 反射解析性能差，读取性能好(struct+offset)

a collection of data science take home challenges
https://github.com/leizton/DS-Take-Home

cpu使用率上不去原因
  cpu使用率超过30%后平响和超时急剧恶化
  cpu均值低方差大

google perftool

lassos net 特征选择

jheap_dump in cpp

combine opt

set_empty_key 原因

https://github.com/leizton/workflow

epoll event

NeRF

tf-cnn ocr
cnn-feature
  A Performance Evaluation of Local Descriptors
  based on OCR
  input拆成多个集合分别train得到不同model, 看model的feature是否有共性
  缩放、旋转对fea的影响
  无监督地做个体识别、img分割
  聚类算法 twitter sim_cluster

combine_opt

混排
https://wulc.me/2023/02/26/%E6%B7%B7%E6%8E%92%E9%82%A3%E4%BA%9B%E4%BA%8B/

time_series
  https://github.com/PacktPublishing/Time-Series-Analysis-with-Python-Cookbook
  https://learning.oreilly.com/library/view/time-series-analysis/9781801075541/B17450_FM_Final_VK.xhtml

std::function internal sizeof

image 四叉树分割

https://tesseract-ocr.github.io/tessdoc/Data-Files.html

推荐系统必读文章
https://github.com/leizton/RSPapers

weather & sk

tcmalloc
https://google.github.io/tcmalloc/design.html

https://github.com/stasi009/PracticalGuidetoRecSys/tree/main/snippets

https://github.com/awesomedata/awesome-public-datasets

https://zhuanlan.zhihu.com/p/423417458

https://google.github.io/tcmalloc/design.html

llm的ability是什么，怎么来的
llm train 理解代码
llm的推理能力与3d-vision的距离推测
llm的推理能力本质来源
llm 学会代码库
llm 的技术难点在于 fine-tune prompt
函数调用链

llm代码阅读/理解/推理 => 函数的摘要
知识图谱约束、常识约束
出现矛盾时用概率解决

FFM
https://tech.meituan.com/2016/03/03/deep-understanding-of-ffm-principles-and-practices.html

向量检索库
https://github.com/microsoft/SPTAG

https://www.shenyanchao.cn/blog/2018/12/04/lucene-fst/

100小球，每次拿1-5颗，拿完者胜，先拿一定取胜的策略

多样性
https://zhuanlan.zhihu.com/p/264652162
DPP https://zhuanlan.zhihu.com/p/95607668