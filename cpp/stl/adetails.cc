#include "util/base.h"

struct Sleeper {
  Sleeper(int sec) {
    sleepSec(sec);
    create_thread_id = currentThreadId();
  }

  ~Sleeper() {
    println("~Sleeper{", create_thread_id, "}");
  }

  uint64_t create_thread_id;
};

def(static_instance_in_func) {
  /*
    从 c++11 开始, sleeper_的构造是线程安全
    https://stackoverflow.com/questions/1661529/is-meyers-implementation-of-the-singleton-pattern-thread-safe

    根据 c++11标准 section 6.7. http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf
    “the concurrent execution shall wait for completion of the initialization”
  */
  auto instance = []() -> Sleeper& {
    static Sleeper sleeper_(5);
    return sleeper_;
  };

  std::set<uint64_t> thread_ids;
  for (int i = 0; i < 10; i++) {
    std::thread th(instance);
    th.detach();
    thread_ids.insert(getThreadId(th));
  }
  sleepSec(1);

  println("main thread: ", currentThreadId());
  println("create sleeper thread: ", instance().create_thread_id);
  println("create sleeper thread: ", instance().create_thread_id);
  println(thread_ids.find(instance().create_thread_id) != thread_ids.end());
}

main_run;