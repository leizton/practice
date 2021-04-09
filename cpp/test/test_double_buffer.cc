#include "util/base.h"

int main() {
  DoubleBuffer<std::string> buf;

  auto* ina_obj = buf.getInactiveObj();
  *ina_obj = "a";
  assert_eq(0u, buf.getActiveIndex());
  buf.switchObj();
  assert_eq(1u, buf.getActiveIndex());

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

  auto update_fn = [&buf]() {
    auto* ina_obj = buf.getInactiveObj();
    *ina_obj = "b";
    buf.switchObj();
  };

  assert_eq(1u, buf.getActiveIndex());
  std::thread update_th(update_fn);
  update_th.join();
  assert_eq(0u, buf.getActiveIndex());

  assert_eq(std::string("a"), *obj_2);
  assert_eq(std::string("b"), *buf.getActiveObj());

  assert_eq(1u, buf.getInactiveUseCount());
  obj_2.reset();
  assert_eq(0u, buf.getInactiveUseCount());
}
