#include "util/base.h"


def(static_var_in_func) {
  /*
    从 c++11 开始, _aoo的构造是线程安全
    https://stackoverflow.com/questions/1661529/is-meyers-implementation-of-the-singleton-pattern-thread-safe

    根据 c++11标准 section 6.7. http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf
    “the concurrent execution shall wait for completion of the initialization”
  */
  auto instance = []() -> Aoo& {
    static Aoo _aoo;
    return _aoo;
  };
  print(instance().id);
}


main_run;