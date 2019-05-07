#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

#include "chrono_util.h"

int main(int argc, const char* argv[]) {
  const int totalIncrNum = 2000000;
  const int threadNum = 1;
  const int incrNum = totalIncrNum / threadNum;

  std::vector<std::thread> threads;
  int cnt = 0;
  std::mutex mtx;

  auto start_tm = ChronoUtil::now();
  for (int i = 0; i < threadNum; i++) {
    threads.emplace_back([&cnt, &mtx, &incrNum] {
      for (int i = 0; i < incrNum; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        for (int j = 0; j < 1000; j++);
        cnt++;
      }
    });
  }

  for (auto& th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }

  auto end_tm = ChronoUtil::now();
  std::cout << "cnt: " << cnt << std::endl;
  std::cout << "thread num: " << threadNum << std::endl;
  std::cout << "cost: " << ChronoUtil::timeDiff(end_tm, start_tm) << std::endl;

  /*
   * threadNum    cost
   * ------------|----
   * 1            3454    no lock, lock cost 27ns
   * 1            3507
   * 2            6005
   * 4            6576
   * 8            6828
   * 16           6588
   * 32           6120
   * 64           6019
  */

}
