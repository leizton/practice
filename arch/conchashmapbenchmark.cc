#include "util/base.h"

using namespace std;

inline std::size_t getHash(const std::string& s) {
  return std::hash<std::string>{}(s);
}

void testKeys(vector<string>& keys, const int num) {
  keys.reserve(num);
  for (int i = 0; i < num; i++) {
    keys.push_back(to_string(i) + to_string(num - i));
  }
}

void hashMapBenchmark(vector<string>& keys, const int runNum, const int readNum) {
  unordered_map<string, string> data;
  const string value("abc123");
  auto start_tm = ChronoUtil::now();

  for (int i = 0; i < runNum; i++) {
    for (auto& e : keys) {
      data.insert({e, value});
    }
    for (int j = 0; j < readNum; j++) {
      for (auto& e : keys) {
        if (data.count(e) == 0) {
          cout << "error" << endl;
          return;
        }
      }
    }
    data.clear();
  }

  auto end_tm = ChronoUtil::now();
  auto cost = ChronoUtil::timeDiff(end_tm, start_tm);
  std::cout << "cost=" << cost << std::endl;
}

template <class K, class V>
class ThreadSafeMap {
 public:
  bool insert(const std::pair<K, V>& e) {
    std::unique_lock<std::shared_mutex> lock(mtx_);
    return data_.insert(e).second;
  }

  int count(const K& key) {
    std::shared_lock<std::shared_mutex> lock(mtx_);
    return data_.count(key);
  }

  void clear() {
    std::unique_lock<std::shared_mutex> lock(mtx_);
    data_.clear();
  }

 public:
  std::unordered_map<K, V> data_;
  std::shared_mutex mtx_;
};

template <class K, class V>
class ConcHashMapV1 {
 public:
  bool insert(const std::pair<K, V>& e) {
    auto idx = getHash(e.first) % kBucketNum;
    return buckets_[idx].insert(e);
  }

  int count(const K& key) {
    auto idx = getHash(key) % kBucketNum;
    return buckets_[idx].count(key);
  }

  void clear() {
    for (std::size_t idx = 0; idx < kBucketNum; idx++) {
      buckets_[idx].clear();
    }
  }

 private:
  static const std::size_t kBucketNum = 511;
  ThreadSafeMap<K, V> buckets_[kBucketNum];
};

void threadSafedHashMapBenchmark(vector<string>& keys, const int runNum,
                                 const int readNum, const int threadNum) {
  // ThreadSafeMap<string, string> data;
  ConcHashMapV1<string, string> data;
  const int taskNum = keys.size() / threadNum;
  const string value("abc123");
  auto start_tm = ChronoUtil::now();

  std::vector<std::thread> threads;
  for (int id = 0; id < threadNum; id++) {
    const int keyBegin = id * taskNum;
    const int keyEnd = std::min((id + 1) * taskNum, (int) keys.size());
    auto f = [&keys, &value, &data, &runNum] (const int keyBegin, const int keyEnd) {
      for (int i = 0; i < runNum; i++) {
        for (int j = keyBegin; j < keyEnd; j++) {
          data.insert({keys[j], value});
        }
      }
    };
    auto g = std::bind(f, keyBegin, keyEnd);
    threads.push_back(std::thread(g));
  }
  for (auto& th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }

  auto end_tm1 = ChronoUtil::now();
  auto cost1 = ChronoUtil::timeDiff(end_tm1, start_tm);

  threads.clear();
  for (int id = 0; id < threadNum; id++) {
    threads.push_back(std::thread([&keys, &data, &runNum] {
      for (int i = 0; i < runNum; i++) {
        for (auto& e : keys) {
          if (data.count(e) == 0) {
            cout << "error" << endl;
            return;
          }
        }
      }
    }));
  }
  for (auto& th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }

  auto end_tm2 = ChronoUtil::now();
  auto cost2 = ChronoUtil::timeDiff(end_tm2, start_tm);
  std::cout << "cost1=" << cost1 << std::endl;
  std::cout << "cost2=" << cost2 << std::endl;
}

int main() {
  const int mapSize = 40000;
  vector<string> keys;
  testKeys(keys, mapSize);

  // -O0: 20086
  // -O3: 7373
  hashMapBenchmark(keys, 200, 10);

  // ThreadSafeMap
  //   -O0: cost1=29273, cost2=45317
  //   -O3: cost1=18442, cost2=30211
  // ConcHashMapV1
  //   -O3: cost1=476, cost2=1740
  threadSafedHashMapBenchmark(keys, 200, 10, 4);
}
