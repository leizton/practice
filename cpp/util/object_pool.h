#pragma once

#include "concurrent.h"

template <class T> class ObjectPool;

template <class T>
struct ObjectHolder {
  T* obj = nullptr;
  ObjectPool<T>* pool = nullptr;

  ~ObjectHolder();
};

template <class T>
class ObjectPool {
public:
  typedef T Object;

  ~ObjectPool();

  void clear();
  void reserve(int n);
  size_t size();

  bool allocObject(ObjectHolder<ObjectPool<T>::Object>& holder);
  bool allocObject(ObjectPool<T>::Object*& obj);
  void freeObject(ObjectPool<T>::Object* obj);

private:
  std::mutex pool_mtx_;
  std::vector<ObjectPool<T>::Object*> pool_;
};

template <class T>
ObjectPool<T>::~ObjectPool() {
  clear();
}

template <class T>
void ObjectPool<T>::clear() {
  std::lock_guard<std::mutex> lk(pool_mtx_);
  for (Object* obj : pool_) {
    delete obj;
  }
  pool_.clear();
}

template <class T>
size_t ObjectPool<T>::size() {
  std::lock_guard<std::mutex> lk(pool_mtx_);
  return pool_.size();
}

template <class T>
void ObjectPool<T>::reserve(int n) {
  std::lock_guard<std::mutex> lk(pool_mtx_);
  pool_.reserve(std::max(1, n));
}

template <class T>
bool ObjectPool<T>::allocObject(ObjectHolder<ObjectPool<T>::Object>& holder) {
  holder.pool = this;
  std::lock_guard<std::mutex> lk(pool_mtx_);
  if (!pool_.empty()) {
    holder.obj = pool_.back();
    pool_.pop_back();
    return true;
  } else {
    return false;
  }
}

template <class T>
bool ObjectPool<T>::allocObject(ObjectPool<T>::Object*& obj) {
  std::lock_guard<std::mutex> lk(pool_mtx_);
  if (!pool_.empty()) {
    obj = pool_.back();
    pool_.pop_back();
    return true;
  } else {
    return false;
  }
}

template <class T>
void ObjectPool<T>::freeObject(ObjectPool<T>::Object* obj) {
  if (obj) {
    std::lock_guard<std::mutex> lk(pool_mtx_);
    pool_.push_back(obj);
  }
}

template <class T>
ObjectHolder<T>::~ObjectHolder() {
  if (pool) {
    pool->freeObject(obj);
  }
}

template <class T>
class AtomicObjectPool {
public:
  typedef T Object;

  AtomicObjectPool();
  ~AtomicObjectPool();

  // 初始化 n 个元素, 之后不再增加
  template <class ...Args>
  void init(size_t n, Args ...args);

  size_t size() const;

  // 返回 false 表示没有空闲
  bool allocObject(Object*& obj, int& index);

  // 返回 false 表示归还失败, 需要调用方 delete obj
  bool freeObject(Object* obj, int index);

private:
  std::vector<Object*> pool_;
  std::vector<AtomicWrapper<bool>> is_free_;
  std::atomic<uint32_t> next_;
  size_t real_size_;
  size_t mod_size_;
};

template <class T>
AtomicObjectPool<T>::AtomicObjectPool()
    : next_(0)
    , real_size_(0)
    , mod_size_(0) {
}

template <class T>
AtomicObjectPool<T>::~AtomicObjectPool() {
  for (size_t i = 0; i < pool_.size(); i++) {
    delete pool_[i];
    pool_[i] = nullptr;
  }
  pool_.clear();
  next_ = 0;
  real_size_ = 0;
  mod_size_ = 0;
}

template <class T>
template <class ...Args>
void AtomicObjectPool<T>::init(size_t n, Args ...args) {
  real_size_ = 2;
  while (real_size_ < n) {
    real_size_ *= 2;
  }
  mod_size_ = real_size_ - 1;
  pool_.reserve(real_size_);
  is_free_.reserve(real_size_);
  for (size_t i = 0; i < real_size_; i++) {
    pool_.push_back(new AtomicObjectPool<T>::Object(args...));
    is_free_.push_back(AtomicWrapper<bool>());
    is_free_.back().v.store(true);
  }
}

template <class T>
size_t AtomicObjectPool<T>::size() const {
  return real_size_;
}

template <class T>
bool AtomicObjectPool<T>::allocObject(AtomicObjectPool<T>::Object*& obj, int& index) {
  index = -1;
  for (size_t i = 0; i < real_size_; i++) {
    uint32_t j = (next_.fetch_add(1)) & mod_size_;
    if (is_free_[j].v.load() == false) continue;
    bool expect_free = true;
    if (is_free_[j].v.compare_exchange_strong(expect_free, false)) {
      obj = pool_[j];
      index = (int)j;
      return true;
    }
  }
  return false;
}

template <class T>
bool AtomicObjectPool<T>::freeObject(AtomicObjectPool<T>::Object* obj, int index) {
  if (0 <= index && index < (int)real_size_) {
    if (is_free_[index].v.load() == false) {
      pool_[index] = obj;
      is_free_[index].v.store(true);
      return true;
    }
  }
  return false;
}
