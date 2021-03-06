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
    if (num_ <= 0) {
      cond_.notify_all();
    }
  }

  void wait() {
    std::unique_lock<std::mutex> lk(mtx_);
    cond_.wait(lk, [this] { return this->num_ <= 0; });
  }

  void countDown(int32_t n) {
    std::lock_guard<std::mutex> lk(mtx_);
    num_ -= n;
    if (num_ <= 0) {
      cond_.notify_all();
    }
  }

private:
  int32_t num_;
  std::mutex mtx_;
  std::condition_variable cond_;
};
