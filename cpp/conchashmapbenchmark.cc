#include <iostream>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include "chrono_util.h"

using namespace std;

void testKeys(vector<string>& keys, const int num) {
  keys.reserve(num);
  for (int i = 0; i < num; i++) {
    keys.push_back(to_string(i) + to_string(num - i));
  }
}

void hashMapBenchmark(vector<string>& keys, const int runNum, const int readNum) {
  unordered_map<string, string> data;
  auto start_tm = ChronoUtil::now();

  for (int i = 0; i < runNum; i++) {
    for (auto& e : keys) {
      data[e] = "abc123";
    }
    for (int j = 0; j < readNum; j++) {
      for (auto& e : keys) {
        if (data.find(e) == data.end()) {
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
  void put(const K& k, const V& v) {
    std::unique_lock<std::shared_mutex> lock(mtx_);
    data_[k] = v;
  }

  bool contains(const K& k) {
    std::shared_lock<std::shared_mutex> lock(mtx_);
    return data_.find(k) != data_.end();
  }

  void clear() {
    std::unique_lock<std::shared_mutex> lock(mtx_);
    data_.clear();
  }

 public:
  std::unordered_map<K, V> data_;
  std::shared_mutex mtx_;
};

void threadSafedHashMapBenchmark(vector<string>& keys, const int runNum,
                                 const int readNum, const int threadNum) {
  ThreadSafeMap<string, string> data;
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
          data.put(keys[j], value);
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
          if (!data.contains(e)) {
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

  hashMapBenchmark(keys, 200, 10);  // 20086
  threadSafedHashMapBenchmark(keys, 200, 10, 4);  // cost1=29273, cost2=45317
}
