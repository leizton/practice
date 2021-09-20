#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>

#include "time_utils.h"

/**
 * 两种限流算法
 *   令牌桶 漏桶
 * 区别
 *   令牌桶：允许一定的流量突发
 *   漏桶：不允许流量突发，强行限在阈值内
 * 实现原理
 *   令牌桶：按固定速率往桶中放令牌，每个请求取一个令牌，令牌累积从而允许突发
 *   漏桶：请求流入漏斗，漏斗流出速率固定，流出漏斗的请求被处理，溢出漏斗的请求被丢弃
 */

/**
 * 令牌桶算法
 * @ref guava::RateLimiter
 */
class RateLimiter {
public:
  RateLimiter() {
    setRate(1);
  }

  RateLimiter(int qps) {
    setRate(qps);
  }

  /**
   * @example：
   *   RateLimiter(10, 1) 表示 1秒10个令牌
   *   RateLimiter(1, 10) 表示 10秒1个令牌
   *   permit 即令牌
   */
  RateLimiter(int permit_num_per_span, int span_sec) {
    setRate(permit_num_per_span, span_sec);
  }

  void setRate(int qps) {
    setRate(qps, 1);
  }

  void setRate(int permit_num_per_span, int span_sec) {
    std::lock_guard<std::mutex> lock(mtx_);
    permit_num_per_span_ = std::max(1, permit_num_per_span);
    span_sec_ = std::max(1, span_sec);
    max_permit_ = std::max(1, permit_num_per_span / span_sec);
    permit_interval_us_ = 1000000ll * int64_t(span_sec) / int64_t(permit_num_per_span);
    last_permit_ts_ = nowUs();
    stored_permit_num_ = 0;
  }

  void syncAcquire(int acq_num = 1) {
    if (acq_num <= 0) return;
    std::lock_guard<std::mutex> lock(mtx_);
    int64_t now = nowUs();
    if (tryAcq(now, acq_num)) return;
    int64_t target = last_permit_ts_ + (acq_num - stored_permit_num_) * permit_interval_us_;
    stored_permit_num_ = 0;
    last_permit_ts_ = target;
    if (target > now) sleepUs(target - now);
  }

  /**
   * @return permit num
   */
  int tryAcquire(int acq_num = 1, int64_t timeout_us = 0) {
    if (acq_num <= 0) return 0;
    std::lock_guard<std::mutex> lock(mtx_);
    int64_t now = nowUs();
    if (tryAcq(now, acq_num)) return acq_num;

    int acqed_num = stored_permit_num_;
    stored_permit_num_ = 0;
    if (timeout_us <= 0) return acqed_num;

    int64_t target = last_permit_ts_ + (acq_num - acqed_num) * permit_interval_us_;
    if (target > now + timeout_us) {
      int new_permit_num = (now + timeout_us - last_permit_ts_) / permit_interval_us_;
      new_permit_num = std::min(max_permit_, new_permit_num);
      last_permit_ts_ += new_permit_num * permit_interval_us_;
      acqed_num += new_permit_num;
      if (acqed_num < acq_num && last_permit_ts_ < now + timeout_us) {
        target = now + timeout_us;
      }
    } else {
      acqed_num = acq_num;
      last_permit_ts_ = target;
    }
    if (target > now) sleepUs(target - now);
    return acqed_num;
  }

private:
  bool tryAcq(int64_t now, int acq_num) {
    if (now > last_permit_ts_) {
      int new_permit_num = (now - last_permit_ts_) / permit_interval_us_;
      new_permit_num = std::min(max_permit_, new_permit_num);
      stored_permit_num_ += new_permit_num;
      // 如果整除后 new_permit_num 是 0, 则 last_permit_ts_ 实际上不变
      last_permit_ts_ += new_permit_num * permit_interval_us_;
    }
    if (stored_permit_num_ >= acq_num) {
      stored_permit_num_ -= acq_num;
      return true;
    } else {
      return false;
    }
  }

private:
  int permit_num_per_span_;
  int span_sec_;
  int max_permit_;
  int64_t permit_interval_us_; // 相邻permit的时间间隔, 控制令牌投放速率
  int64_t last_permit_ts_; // 上一个permit的时间戳
  int stored_permit_num_; // 蓄水池里的令牌数
  std::mutex mtx_;
};
