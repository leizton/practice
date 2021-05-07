#pragma once

#include <thread>

inline uint64_t currentThreadId() {
  return (uint64_t)pthread_self();
}

inline uint64_t getThreadId(const std::thread& th) {
  return (uint64_t)const_cast<std::thread&>(th).native_handle();
}
