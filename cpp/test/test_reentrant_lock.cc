#include "util/base.h"

int main() {
  ReentrantLock lk;
  lk.lock();
  print(1);

  {
    std::lock_guard<ReentrantLock> lkg(lk);
    print(2);
  }

  assert_T(lk.try_lock());
  lk.unlock();

  lk.unlock();

  try {
    lk.unlock();
    assert_T(false);
  } catch (std::exception& ex) {
    assert_T(true);
  }

  return 0;
}
