#include "util/base.h"


def(construct_shared_ptr) {
  int v = 0;
  shared_ptr<void> ptr(nullptr, [&v](void* p) {
    assert_eq(1, v);
  });
  v++;
}


def(shared_ptr_void) {
  AooPtr pa1(new Aoo("321"));
  shared_ptr<void> p1 = pa1;
  assert_eq(2, pa1.use_count());
  pa1.reset();
  assert_eq(1, p1.use_count());

  AooPtr pa2(p1, static_cast<Aoo*>(p1.get()));
  assert_eq(2, pa2.use_count());
  p1.reset();
  assert_eq(1, pa2.use_count());
}


main_run;