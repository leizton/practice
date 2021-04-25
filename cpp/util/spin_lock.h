#pragma once

#include <atomic>

class SpinLock {
 public:
  void lock() {
    while (lock_.exchange(LockState::Locked, std::memory_order_relaxed) == LockState::Locked);
        // 保证锁前代码的读写不会运行在锁后, 且锁前的读在锁后可见
    std::atomic_thread_fence(std::memory_order_acquire);
        // 保证锁内的读写不会运行到锁前
  }
  void unlock() {
    lock_.store(LockState::UnLock, std::memory_order_release);
        // 保证锁内的读写不会跑到外面
  }
 private:
  enum class LockState { Locked, UnLock };
  std::atomic<LockState> lock_;
};
