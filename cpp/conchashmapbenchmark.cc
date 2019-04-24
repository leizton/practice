#include <iostream>
#include <string>
#include <unordered_map>

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
  std::cout << "cost: " << cost << std::endl;  // 24712
}

void concHashMapBenchmark(vector<string>& keys, const int runNum,
                          const int readNum, const int threadNum) {
}

int main() {
  const int mapSize = 40000;
  vector<string> keys;
  testKeys(keys, mapSize);

  hashMapBenchmark(keys, 200, 10);
}
