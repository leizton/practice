#pragma once

#include <atomic>
#include <chrono>

template <class DataType>
class DoubleBuffer final {
private:
  static const int MaskShift = 31;
  static const uint32_t Mask = (1u << MaskShift);

public:
  DoubleBuffer(int switch_sleep_us = 100);
  ~DoubleBuffer();
  DoubleBuffer(const DoubleBuffer&) = delete;
  DoubleBuffer& operator=(const DoubleBuffer&) = delete;

  std::shared_ptr<DataType> getActiveObj(); // for read

  DataType* getInactiveObj(); // for write

  void switchObj();

  uint32_t getActiveUseCount() const;
  uint32_t getActiveIndex() const;
  uint32_t getInactiveIndex() const;

private:
  struct Deleter {
    Deleter(std::atomic<uint32_t>& cnt) : cnt_(cnt) {}
    void operator()(DataType*) { cnt_--; }
    std::atomic<uint32_t>& cnt_;
  };

private:
  const int switch_sleep_us_;
  DataType* buf_[2];
  std::atomic<uint32_t> active_idx_;
  Deleter deleter_;
};

template <class DataType>
DoubleBuffer<DataType>::DoubleBuffer(int switch_sleep_us)
    : switch_sleep_us_(switch_sleep_us)
    , deleter_(active_idx_) {
  buf_[0] = new DataType;
  buf_[1] = new DataType;
  active_idx_ = Mask;
}

template <class DataType>
DoubleBuffer<DataType>::~DoubleBuffer() {
  delete buf_[0];
  buf_[0] = nullptr;
  delete buf_[1];
  buf_[1] = nullptr;
}

template <class DataType>
std::shared_ptr<DataType> DoubleBuffer<DataType>::getActiveObj() {
  uint32_t idx = ((++active_idx_) & Mask) >> MaskShift;
  return std::shared_ptr<DataType>(buf_[idx], deleter_);
}

template <class DataType>
DataType* DoubleBuffer<DataType>::getInactiveObj() {
  uint32_t idx = getActiveIndex();
  return buf_[1u - idx];
}

template <class DataType>
void DoubleBuffer<DataType>::switchObj() {
  uint32_t expect = active_idx_.load() & Mask;
  const uint32_t val = expect ^ Mask;
  while (!active_idx_.compare_exchange_strong(expect, val)) {
    expect = active_idx_.load() & Mask;
    std::this_thread::sleep_for(std::chrono::duration<int, std::micro>(switch_sleep_us_));
  }
}

template <class DataType>
uint32_t DoubleBuffer<DataType>::getActiveUseCount() const {
  return active_idx_.load() & (~Mask);
}

template <class DataType>
uint32_t DoubleBuffer<DataType>::getActiveIndex() const {
  return (active_idx_.load() & Mask) >> MaskShift;
}

template <class DataType>
uint32_t DoubleBuffer<DataType>::getInactiveIndex() const {
  return 1u - getActiveIndex();
}
