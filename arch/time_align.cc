#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>

uint64_t NowMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t NowSec() {
  return std::chrono::duration_cast<std::chrono::seconds>(
    std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
  const int periodSec = 5;

  std::chrono::milliseconds once_wait(50);
  uint64_t last_tsec = NowSec();

  while (true) {
    std::this_thread::sleep_for(once_wait);
    last_tsec = NowSec();
    if (last_tsec % 60 == 0) {
      break;
    }
  }

  while (true) {
    while (true) {
      std::this_thread::sleep_for(once_wait);  // 多线程环境下不能sleep太久, sleep太久会更容易有偏差
      uint64_t tsec = NowSec();
      if (tsec != last_tsec && (tsec % periodSec) == 0) {
        last_tsec = tsec;
        break;
      }
    }

    uint64_t ts = NowMs();
    std::time_t t = ts / 1000;
    char buf[100];
    std::strftime(buf, sizeof(buf), "%A %c", std::localtime(&t));
    int rand_wait = std::rand() % (2 * periodSec * 1000);
    std::cout << std::string(buf) << " " << (ts % 1000) << " " << rand_wait << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(rand_wait));
  }
}
