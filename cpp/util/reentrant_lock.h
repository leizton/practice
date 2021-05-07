#pragma once

#include <atomic>
#include <mutex>

#include "thread_utils.h"

class ReentrantLock {
public:
  ReentrantLock()
      : held_thread_(0)
      , lock_num_(0) {}

  void lock() {
    const uint64_t curr_thread = currentThreadId();
    if (held_thread_ == curr_thread) {
      lock_num_++;
      return;
    }
    mtx_.lock();
    held_thread_ = curr_thread;
    lock_num_ = 1;
  }

  bool try_lock() {
    const uint64_t curr_thread = currentThreadId();
    if (held_thread_ == curr_thread) {
      lock_num_++;
      return true;
    }
    bool locked = mtx_.try_lock();
    if (locked) {
      held_thread_ = curr_thread;
      lock_num_ = 1;
      return true;
    } else {
      return false;
    }
  }

  void unlock() {
    if (held_thread_ != currentThreadId()) {
      throw std::logic_error("not_held_lock");
    }
    if (--lock_num_ == 0) {
      held_thread_ = 0;
    }
  }

private:
  std::mutex mtx_;
  std::atomic<uint64_t> held_thread_;
  int lock_num_;
};
