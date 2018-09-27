#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>

typedef std::chrono::time_point<std::chrono::system_clock> time_point;

inline time_point now() {
    return std::chrono::system_clock::now();
}

inline uint64_t timeDiff(const time_point& end, const time_point& start) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main(int argc, const char* argv[]) {
  int cnt = 0;
  std::mutex mtx;

  const int totalIncrNum = 4000000;
  const int threadNum = atoi(argv[1]);
  const int incrNum = totalIncrNum / threadNum;
  std::vector<std::thread> threads;

  auto start_tm = now();
  for (int i = 0; i < threadNum; i++) {
    threads.push_back(std::thread([&cnt, &mtx, &incrNum] {
      for (int i = 0; i < incrNum; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        for (int j = 0; j < 1000; j++);
        cnt++;
      }
    }));
  }

  for (auto& th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }

  auto end_tm = now();
  std::cout << "cnt: " << cnt << std::endl;
  std::cout << "thread num: " << threadNum << std::endl;
  std::cout << "cost: " << timeDiff(end_tm, start_tm) << std::endl;

  return 0;
}
