#include "util/base.h"

void testSyncAcquire() {
  Timer timer;
  int64_t cost = 0;
  RateLimiter limiter;

  limiter.setRate(41);
  timer.interval();
  for (int i = 0; i < 41 * 2; i++) {
    limiter.syncAcquire();
  }
  cost = timer.interval();
  assert_T(1990 < cost && cost < 2010);
  LOG_INFO() << cost;

  limiter.setRate(13);
  timer.interval();
  for (int i = 0; i < 13; i++) {
    limiter.syncAcquire();
  }
  cost = timer.interval();
  assert_T(990 < cost && cost < 1010);
  LOG_INFO() << cost;

  limiter.setRate(12);
  timer.interval();
  for (int i = 0; i < 4; i++) {
    limiter.syncAcquire(3);
  }
  cost = timer.interval();
  assert_T(990 < cost && cost < 1010);
  LOG_INFO() << cost;

  limiter.setRate(231);
  timer.interval();
  for (int i = 0; i < 231; i++) {
    limiter.syncAcquire();
  }
  cost = timer.interval();
  assert_T(990 < cost && cost < 1010);
  LOG_INFO() << cost;

  limiter.setRate(1, 2);
  timer.interval();
  limiter.syncAcquire();
  cost = timer.interval();
  assert_T(1990 < cost && cost < 2010);
  LOG_INFO() << cost;
}

void testTryAcquire() {
  Timer timer;
  int64_t cost = 0;
  RateLimiter limiter;

  limiter.setRate(5);
  timer.interval();
  assert_eq(0, limiter.tryAcquire());
  sleepMs(200);
  assert_eq(1, limiter.tryAcquire());
  cost = timer.interval();
  assert_T(190 < cost && cost < 210);
  LOG_INFO() << cost;

  timer.interval();
  sleepMs(100);
  assert_eq(0, limiter.tryAcquire(1, 50 * 1000));
  assert_eq(1, limiter.tryAcquire(1, 50 * 1000));
  cost = timer.interval();
  assert_T(190 < cost && cost < 210);
  LOG_INFO() << cost;

  timer.interval();
  assert_eq(2, limiter.tryAcquire(2, 400 * 1000));
  cost = timer.interval();
  assert_T(390 < cost && cost < 410);
  LOG_INFO() << cost;
}

int main() {
  testSyncAcquire();
  testTryAcquire();
}
