#include "util/base.h"

int main() {
  DoubleBuffer<std::string> buf;

  auto* ina_obj = buf.getInactiveObj();
  *ina_obj = "a";
  assert_eq(1u, buf.getActiveIndex());
  buf.switchObj();
  assert_eq(0u, buf.getActiveIndex());

  assert_eq(0u, buf.getActiveUseCount());
  assert_eq(std::string("a"), *buf.getActiveObj());
  assert_eq(0u, buf.getActiveUseCount());

  auto obj_1 = buf.getActiveObj();
  assert_eq(1u, buf.getActiveUseCount());
  assert_eq(std::string("a"), *obj_1);

  auto obj_2 = buf.getActiveObj();
  assert_eq(2u, buf.getActiveUseCount());

  obj_1.reset();
  assert_eq(1u, buf.getActiveUseCount());

  std::atomic<bool> change_done = false;

  auto change = [&buf, &change_done]() {
    auto* ina_obj = buf.getInactiveObj();
    *ina_obj = "b";
    buf.switchObj();
    change_done = true;
  };
  std::thread th(change);

  sleepSec(1);
  assert_eq(0u, buf.getActiveIndex());

  assert_F(change_done); // happens before change-thread complete
  obj_2.reset();
  assert_eq(0u, buf.getActiveUseCount());

  th.join();
  assert_T(change_done);
  assert_eq(1u, buf.getActiveIndex());
  assert_eq(std::string("b"), *buf.getActiveObj());
}
