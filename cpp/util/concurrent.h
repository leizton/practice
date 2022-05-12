#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class CountDownLatch {
public:
  CountDownLatch() : count_(0) {}
  CountDownLatch(int32_t n) : count_(n) {}

  // @return: true if succ else false
  bool wait(int64_t timeout_us = -1) {
    std::unique_lock<std::mutex> lk(mtx_);
    if (timeout_us <= 0) {
      cond_.wait(lk, [this] { return this->count_ <= 0; });
      return true;
    } else {
      return cond_.wait_for(lk, std::chrono::microseconds(timeout_us),
                            [this] { return this->count_ <= 0; });
    }
  }

  void countDown(int32_t n = 1) {
    std::lock_guard<std::mutex> lk(mtx_);
    count_ -= n;
    tryNotify();
  }

  void countDownToZero() {
    reset(0);
  }

  void reset(int32_t n) {
    std::lock_guard<std::mutex> lk(mtx_);
    count_ = n;
    tryNotify();
  }

  int getCount() const {
    return count_;
  }

private:
  void tryNotify() {
    if (count_ <= 0) {
      cond_.notify_all();
    }
  }

private:
  int32_t count_;
  std::mutex mtx_;
  std::condition_variable cond_;
};

template <class T>
struct AtomicWrapper {
  std::atomic<T> v;

  AtomicWrapper()
      : v() {}
  AtomicWrapper(const AtomicWrapper& a)
      : v(a.v.load()) {}
  AtomicWrapper& operator=(const AtomicWrapper& other) {
    v.store(other.v.load());
  }
};
