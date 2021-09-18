#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class CountDownLatch {
public:
  CountDownLatch() : num_(0) {}
  CountDownLatch(int32_t n) : num_(n) {}

  void reset(int32_t n) {
    std::lock_guard<std::mutex> lk(mtx_);
    num_ = n;
    tryNotify();
  }

  // @return: true if timeout else false
  bool wait(int64_t timeout_us = -1) {
    std::unique_lock<std::mutex> lk(mtx_);
    if (timeout_us <= 0) {
      cond_.wait(lk, [this] { return this->num_ <= 0; });
      return false;
    } else {
      bool ok = cond_.wait_for(lk, std::chrono::microseconds(timeout_us),
                               [this] { return this->num_ <= 0; });
      return !ok;
    }
  }

  void countDown(int32_t n = 1) {
    std::lock_guard<std::mutex> lk(mtx_);
    num_ -= n;
    tryNotify();
  }

private:
  void tryNotify() {
    if (num_ <= 0) {
      cond_.notify_all();
    }
  }

private:
  int32_t num_;
  std::mutex mtx_;
  std::condition_variable cond_;
};
