#pragma once

#include <list>
#include <mutex>

template <class T>
class BlockingQueue {
 public:
  void push(const T& elem);
  bool pop(T& elem);
  bool empty();
  size_t size();

 private:
  std::mutex mtx_;
  std::list<T> que_;
  volatile size_t size_ = 0u;
};

template <class T>
void BlockingQueue<T>::push(const T& elem) {
  std::lock_guard<std::mutex> lk(mtx_);
  que_.push_back(elem);
  size_++;
}

template <class T>
bool BlockingQueue<T>::pop(T& elem) {
  std::lock_guard<std::mutex> lk(mtx_);
  if (empty()) return false;
  elem = que_.front();
  que_.pop_front();
  return true;
}

template <class T>
bool BlockingQueue<T>::empty() {
  return size_ == 0u;
}

template <class T>
size_t BlockingQueue<T>::size() {
  return size_;
}
