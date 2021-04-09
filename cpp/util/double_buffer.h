#pragma once

#include <atomic>
#include <chrono>

/**
 * About the implement, the operator of switch & getActive is lockless.
 * However, getActive may return different instances at the same time.
 */
template <class DataType>
class DoubleBuffer final {
public:
  DoubleBuffer();
  ~DoubleBuffer();
  DoubleBuffer(const DoubleBuffer&) = delete;
  DoubleBuffer& operator=(const DoubleBuffer&) = delete;

  std::shared_ptr<DataType> getActiveObj(); // for read

  /**
   * The update-procedure as follow:
   *   1. LOCK
   *   2. T* inactive_obj = getInactiveObj()
   *   3. modify(inactive_obj)
   *   4. switchObj()
   *   5. UNLOCK
   * update-procedure must be protected by mutex
   */
  DataType* getInactiveObj(); // for write
  void switchObj();

  uint32_t getActiveIndex() const { return active_idx_.load(); }
  uint32_t getInactiveIndex() const { return 1u - getActiveIndex(); }

  uint32_t getActiveUseCount() const;
  uint32_t getInactiveUseCount() const;

private:
  struct Counter {
    Counter() : cnt(0) {}
    std::atomic<uint32_t> cnt;
    // wait cnt return to zero
    void waitZero() {
      while (cnt.load() > 0u) {
        std::this_thread::sleep_for(std::chrono::duration<int, std::micro>(100));
      }
    }
  };

  struct Deleter {
    void operator()(DataType*) { (*cnt)--; }
    std::atomic<uint32_t>* cnt;
  };

private:
  std::atomic<uint32_t> active_idx_;
  DataType* buf_[2];
  Counter counter_[2];
  Deleter deleter_[2];
};

template <class DataType>
DoubleBuffer<DataType>::DoubleBuffer()
    : active_idx_(0) {
  buf_[0] = new DataType;
  buf_[1] = new DataType;
  deleter_[0].cnt = &counter_[0].cnt;
  deleter_[1].cnt = &counter_[1].cnt;
}

template <class DataType>
DoubleBuffer<DataType>::~DoubleBuffer() {
  for (int i = 0; i < 2; i++) {
    counter_[i].waitZero();
    delete buf_[i];
    buf_[i] = nullptr;
  }
}

template <class DataType>
std::shared_ptr<DataType> DoubleBuffer<DataType>::getActiveObj() {
  const uint32_t i = getActiveIndex();
  counter_[i].cnt++;
  return std::shared_ptr<DataType>(buf_[i], deleter_[i]);
}

template <class DataType>
DataType* DoubleBuffer<DataType>::getInactiveObj() {
  const uint32_t i = getInactiveIndex();
  counter_[i].waitZero();
  return buf_[i];
}

template <class DataType>
void DoubleBuffer<DataType>::switchObj() {
  active_idx_.store(getInactiveIndex());
}

template <class DataType>
uint32_t DoubleBuffer<DataType>::getActiveUseCount() const {
  return counter_[getActiveIndex()].cnt.load();
}

template <class DataType>
uint32_t DoubleBuffer<DataType>::getInactiveUseCount() const {
  return counter_[getInactiveIndex()].cnt.load();
}
