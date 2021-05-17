关注数据，核心流转数据的修改即是核心流程

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

编程模型
  - 偏于关注数据抽象
    面向对象
  - 偏于关注数据处理
    接口命令式: 面向接口编程
    数据流声明式: java8 stream; 针对某一种数据类型的迭代操作, 数据类型 List<T>
      [flink 数据流编程模型](./adoc/flink/Dataflow_Programming_Model.pdf)
      批数据声明式: 接口参数是list型
    响应式函数编程: 数据流声明式的异步化; 事件(数据)驱动式
数据模型
  - 传统结构体: protobuf
  - 列式内存数据: arrow; 无schema批量数据

推荐系统的在线服务
  推荐前端服务: 数据处理流式化, 执行流程图化, 策略部署微服务化, 访问下游异步化;
              公共中间件: ItemStream数据流, 基于ItemStream的算法库, 图执行器, 对接下游的client;
              不同业务场景有不同的具体数据 Context/Config/UserInfo/Item/Request/Response;
              不同业务有不同策略;
  平台服务: 用户平台 召回平台 正排和过滤平台 特征平台 模型排序平台;
           平台指具有跨业务场景提供通用能力, 使用的数据模型具有业务无关性;

特征工程的重要性
  往业务里应用深度模型的主要时间花在特征数据上
  好的特征可以在不同业务间互相借鉴

特征平台的运行时数据模型的逻辑视图
  一张宽表存放一次请求的所有数据
  表的行数是固定的, 等于输入的item_id数
  表的列分group, 每个group对应一个节点, 例如 source列族 对应请求原始参数 包含列有[item_id, user_profile]
  一个节点也可以创建出多个列族, 节点内通过 group.column 取得具体的列数据
  配置构图: 仅需配置节点的 inputs 和 outputs, 框架自动推导节点依赖关系变成图运行, 所以表的列数在运行前也是确定的
  对于list数据, 如果设置multi-record模式, 则表示这个list是多行; 否则是一行

AI算法发展
  专家系统: 手动配置规则; 规则式逻辑
  机器学习: 面向大量样本的数学最大化
  深度学习: 面向大量样本和大参数集的数学最大化, 和机器学习比能学到更多的人类认知规则

uri (identifier) {
  url (locator) https://google.com
  urn (name) isbn:0-486-27557
}

# plot
FusionCharts
Highcharts
Plotly

# sign
㈠㈡㈢㈣㈤㈥㈦㈧㈨㈩
①②③④⑤⑥⑦⑧⑨⑩
⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽
⒈⒉⒊⒋⒌⒍⒎⒏⒐⒑
☆ ★ ☉ ⊕ □ ■ ○ ●
✓ × 
⚽️ 🏀 ⚾️