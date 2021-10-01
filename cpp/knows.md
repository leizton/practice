架构的基本面
  易用性(对业务开发友好, 简洁原则_最少知识原则, 有助于保持高的业务生产力; 易于排查问题, 监控和trace易用)
  可扩展(数据容量、计算处理量可水平扩展)
  高可用(极少的处理失败、无响应, 容错能力强)
  高性能(处理时间足够短)

基础服务和中间件
  -- distribute
  -- storage
  KV: redis etcd
  Relation: mysql
  NoSql: hbase
  Search: ES
  -- middle-ware
  RPC: brpc pb/fb/arrow
  MQ: kafka rocketmq
  TSDB: openTsdb prometheus
  -- compute
  MapReduce
  spark
  flink
  分布式任务调度
  -- codelib
  disruptor
  tcmalloc

c++基础
  stl: 容器/适配器、迭代器、常用算法函数
  effective c++
  effective stl
  并发编程
  leetcode

编程进阶
  设计模式
  流式api与反应式编程

linux
  常用命令
  性能分析工具

推荐相关系统
  特征平台
    用户画像
    user特征服务: user_info user_profile user_fea
    item特征服务: item字段和特征
  召回服务
    item拉链生成(离线过滤排序)和查询(在线term查询)
    向量索引
    u2i/i2i拉链(GraphDB)

推荐相关组件
  本地item缓存
  过滤