#include "util/base.h"

struct Cond {
  virtual ~Cond() {}
  virtual bool isTrue(void* v) = 0;
};

struct IntEqual : public Cond {
  int data_;
  IntEqual(int v) : data_(v) {}
  bool isTrue(void* v) override {
    return data_ == *static_cast<int*>(v);
  }
};

struct IntEq {
  int data_;
  IntEq(int v) : data_(v) {}
  bool isTrue(void* v) {
    return data_ == *static_cast<int*>(v);
  }
};

std::shared_ptr<Cond> buildCond() {
  return std::make_shared<IntEqual>(10);
}

def(callable_perf) {
  /*
      code_eq     80
      raw_eq      80
      bind_eq     2200
      clz_eq      80
      virtual_eq  1400
  */

  int64_t begin_tm, end_tm;
  int ret;

#define TEST(COMM, RUN)                    \
  {                                        \
    ret = 0;                               \
    begin_tm = nowMs();                    \
    for (int i = 0; i < 1000000000; i++) { \
      ret += RUN;                          \
    }                                      \
    end_tm = nowMs();                      \
    print(#COMM, ret, end_tm - begin_tm);  \
  }

#define code_eq_run 10 == i
  TEST(code_eq, code_eq_run);

  auto raw_eq = [](int data, void* v) -> bool {
    return data == *static_cast<int*>(v);
  };
#define raw_eq_run raw_eq(10, &i)
  TEST(raw_eq, raw_eq_run);

  std::function<bool(void*)> bind_eq = std::bind(raw_eq, 10, std::placeholders::_1);
  TEST(bind_eq, bind_eq(&i));

  IntEq clz_eq(10);
  TEST(clz_eq, clz_eq.isTrue(&i));

  auto cond = buildCond();
  TEST(virtual_eq, cond->isTrue(&i));
}

main_run;