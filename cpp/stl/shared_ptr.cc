#include "util/base.h"


def(construct_shared_ptr) {
  int v = 0;
  shared_ptr<void> ptr(nullptr, [&v](void* p) {
    assert_eq(1, v);
  });
  v++;
}


def(reset) {
  int flag = 0;
  shared_ptr<int> p(new int, [&flag](int*) { flag = 1; });
  assert_T(p);
  assert_eq(0, flag);

  p.reset();
  assert_F(p);
  assert_eq(1, flag);

  p.reset(new int, [&flag](int*) { flag = 2; });
  assert_T(p);
  p.reset(new int);
  assert_eq(2, flag);
}


def(assign) {
  auto b = std::make_shared<TestB>(1, 2);

  shared_ptr<TestA> a;
  a = b;
  assert_eq(2, a.use_count());
  assert_eq(2, b.use_count());

  b.reset();
  assert_eq(1, a.use_count());
  assert_eq(0, b.use_count());

  shared_ptr<void> p;
  p = a;
  assert_eq(2, a.use_count());
  assert_eq(2, p.use_count());

  a.reset();
  assert_eq(0, a.use_count());
  assert_eq(1, p.use_count());
  print("end");
}


def(shared_ptr_void) {
  AooPtr pa1(new Aoo);
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