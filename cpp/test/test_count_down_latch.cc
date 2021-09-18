#include "util/base.h"

int main() {
  int test = 0;

  CountDownLatch latch(1);
  std::thread t1([&latch, &test]() {
    sleepSec(2);
    test = 1;
    latch.countDown();
  });
  t1.detach();
  latch.wait();
  assert_eq(1, test);

  latch.reset(1);
  std::thread t2([&latch, &test]() {
    sleepSec(5);
    test = 2;
    latch.countDown();
  });
  t2.detach();
  latch.wait(2 * 1000 * 1000);
  assert_neq(2, test);
}
