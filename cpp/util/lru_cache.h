#pragma once

#include <cstdint>
#include <unordered_map>

template <class K, class V>
class LruCache {
private:
  class ValueHolder;
  using List = std::list<K>;
  using ListIter = typename List::iterator;

  class ValueHolder {
  public:
    V val;

  private:
    ValueHolder(const V& v) : val(v) {}

    ListIter list_iter_;

    friend class LruCache<K, V>;
  };

public:
  using Map = std::unordered_map<K, ValueHolder>;
  using Iter = typename Map::iterator;

public:
  LruCache(uint32_t max_size) : max_size_(max_size == 0 ? UINT32_MAX : max_size) {}

  /**
   * @return
   *  has created new kv.
   *  true, if key not exist; false, if key existed
   */
  bool put(const K& key, const V& val);

  Iter find(const K& key);

  Iter end();

  size_t size() const;

private:
  const uint32_t max_size_;
  Map data_;
  List lst_;
};

template <class K, class V>
bool LruCache<K, V>::put(const K& key, const V& val) {
  auto it = data_.find(key);
  if (it != data_.end()) {
    it->second.val = val;
    lst_.erase(it->second.list_iter_);
    it->second.list_iter_ = lst_.insert(lst_.end(), key);
    return false;
  }

  if (data_.size() >= max_size_) {
    auto erase_it = lst_.begin();
    data_.erase(data_.find(*erase_it));
    lst_.erase(erase_it);
  }

  auto kv = data_.insert({key, val});
  kv.first->second.list_iter_ = lst_.insert(lst_.end(), key);
  return true;
}

template <class K, class V>
typename LruCache<K, V>::Iter LruCache<K, V>::find(const K& key) {
  return data_.find(key);
}

template <class K, class V>
typename LruCache<K, V>::Iter LruCache<K, V>::end() {
  return data_.end();
}

template <class K, class V>
size_t LruCache<K, V>::size() const {
  return data_.size();
}
