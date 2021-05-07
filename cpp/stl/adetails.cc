#include "util/base.h"

struct Sleeper {
  Sleeper(int sec) {
    sleepSec(sec);
    create_thread_id = currentThreadId();
  }

  ~Sleeper() {
    LOG() << "~Sleeper{" << create_thread_id << "}";
  }

  uint64_t create_thread_id;
};

run(static_instance_in_func) {
  /*
    从 c++11 开始, sleeper_的构造是线程安全
    https://stackoverflow.com/questions/1661529/is-meyers-implementation-of-the-singleton-pattern-thread-safe

    根据 c++11标准 section 6.7. http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf
    “the concurrent execution shall wait for completion of the initialization”
  */

  srand(time(NULL));

  auto instance = []() -> Sleeper& {
    sleepMs(100 + (rand() % 500));
    static Sleeper sleeper_(5);
    return sleeper_;
  };

  std::unordered_map<uint64_t, int> thread_ids;
  for (int i = 0; i < 10; i++) {
    std::thread th(instance);
    thread_ids.emplace(getThreadId(th), i);
    th.detach();
  }

  const uint64_t create_thread_id = instance().create_thread_id; // 会等创建线程结束
  LOG() << "createSleeper-th: " << create_thread_id;
  LOG() << "create thread index: " << thread_ids[create_thread_id];
}

main_run;