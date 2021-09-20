# example
~~~java
void main(String[] args) {
  // 流控配置
  // 添加一个20qps的限流
  var rule = new FlowRule();
  rule.setResource("Hello"); // 设置资源名
  rule.setGrade(RuleConstant.FLOW_GRADE_QPS);
  rule.setCount(20);
  FlowRuleManager.loadRules({rule});

  // 使用
  while (true) {
    Entry hello_entry = null;
    try {
      // 第一步：申请一个资源实例
      hello_entry = SphU.entry("Hello"); // 非阻塞
      // 第二步：申请成功后做业务处理
      bizc();
    } catch (BlockException ex) {
      // 申请失败
    } final {
      // 第三步：归还资源
      if (hello_entry != null) hello_entry.exit();
    }
  }
}
~~~
