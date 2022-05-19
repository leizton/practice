#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class AtomicCountDownLatch {
public:
  AtomicCountDownLatch() : count_(0) {}
  AtomicCountDownLatch(int32_t n) : count_(n) {}

  void wait() {
    std::unique_lock<std::mutex> lk(mtx_);
    cond_.wait(lk, [this] { return this->count_.load() <= 0; });
  }

  void countDown(int32_t n) {
    n = count_.fetch_sub(n) - n;
    if (n <= 0) {
      std::lock_guard<std::mutex> lk(mtx_);
      cond_.notify_all();
    }
  }

  void countUp(int32_t n) {
    countDown(-n);
  }

  int32_t getCount() const {
    return count_.load();
  }

private:
  std::atomic<uint32_t> count_;
  std::mutex mtx_;
  std::condition_variable cond_;
};

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
    if (count_ <= 0) cond_.notify_all();
  }

  void countDownToZero() {
    std::lock_guard<std::mutex> lk(mtx_);
    count_ = 0;
    cond_.notify_all();
  }

  int32_t getCount() const {
    return count_;
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
