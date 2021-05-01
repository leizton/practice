可计算问题 图灵机 图灵完备
https://www.zhihu.com/question/20115374

BitFunnel
https://www.jiqizhixin.com/articles/2019-11-20-15

4种容器网络模式
https://blog.csdn.net/qq_38622229/article/details/81840516

日志收集和分析
1. 收集包含指定 keyword 的日志行, 同时过滤掉无需收集的日志
2. agent 做初步解析, 解析结果包括 {app, ip, timestamp, row_id, kv_list}
    row_id 是日志行的唯一标识, 类型是`uint64_t`, 默认用 snowflake 生成
    kv_list 是一个 kv pair 的数组, key value 的类型都是string
3. 通过消息队列发送出去, 如 kafka
4. 用实时流计算框架做OLAP, 如 flink-sql