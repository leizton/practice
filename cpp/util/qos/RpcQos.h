#pragma once

#include <logger.h>

struct CaptureInfo {
  const bool send_fail;
  const bool timeout;
  const bool process_fail;
  const int64_t cost_us;

  CaptureInfo(bool send_fail_, bool timeout_, bool process_fail_, int64_t cost_us_)
      : send_fail(send_fail_)
      , timeout(timeout_)
      , process_fail(process_fail_)
      , cost_us(cost_us_) {
  }
};

/**
 * 支持功能: (1)按滑动窗口内总失败率降级; (2)滑动窗口限流
 * 总失败率(total_fail_rate) = 发送请求失败率(send_fail_rate) + 超时率(timeout_rate) + 处理失败率(process_fail_rate)
 * 输出监控 cost_avg_p90_p95_p99 qps qos_not_pass_rate total_fail_rate send_fail_rate timeout_rate process_fail_rate
 * qos_not_pass_rate: 由于降级而没有请求远程服务的比率
 */
class RpcQos {
public:
  void Jsonize(autil::legacy::Jsonizable::JsonWrapper& json) override;

  bool Init();
  
  const std::string& GetQosName() const { return qos_name_; }
  
  bool CheckPass();

  void Report(const CaptureInfo& info);
  
  void Update();
  
private:
  struct WindowStat {
    int64_t start_ts;
    int request_num;
    int fail_num;
  };

private:
  // 以下自动从配置中获取
  // qos名称
  std::string qos_name_;
  // 滑动窗口移动步长
  int slide_window_step_ms_;
  // 滑动窗口时间长度
  int slide_window_ms_;
  // 滑动窗口qps限流值
  int slide_window_qps_limit_;

  // 滑动窗口大小 = 滑动窗口时间长度 / 滑动窗口移动步长
  int slide_window_size_;
  // 滑动窗口内请求数量限制 = 滑动窗口时间长度 * qps限流值
  int slide_window_request_num_limit_;
  // 各小窗口的统计信息
  std::vector<WindowStat> slide_window_stats_;

public:
  static const int MinSlideWindowStepMs = 10;
};
