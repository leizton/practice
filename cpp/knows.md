架构的基本面
  高可用(极少的请求处理失败)
  高性能(并发性能)
  可扩展(数据容量、请求处理能力可水平扩展)
  易用性(对业务开发友好)

开源中间件
  -- storage
  redis
  mysql
  hbase
  ES
  etcd
  -- communication
  protobuf/flatbuf
  brpc
  kafka
  -- compute
  mapReduce
  spark
  flink
  -- tools
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