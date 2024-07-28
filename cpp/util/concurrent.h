#pragma once

#include <atomic>

template <class T>
struct AtomicWrapper {
  std::atomic<T> v;

  AtomicWrapper() : v() {}
  AtomicWrapper(const AtomicWrapper& a) : v(a.v.load()) {}
  AtomicWrapper& operator=(const AtomicWrapper& other) {
    v.store(other.v.load());
  }
};
