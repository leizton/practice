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
      # g++ -O3
      code_eq                 1
      lambda_eq               1
      class_eq                1
      virtual_eq              3
      function_eq             10
      bind_lambda_eq          12
      bind_function_eq        16

      # g++ -O2
      code_eq                 3
      lambda_eq               3
      class_eq                3
      virtual_eq              10
      function_eq             10
      bind_lambda_eq          12
      bind_function_eq        16
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

  TEST(code_eq, 10 == i);

  auto lambda_eq = [](int data, void* v) -> bool {
    return data == *static_cast<int*>(v);
  };
#define lambda_eq_run lambda_eq(10, &i)
  TEST(lambda_eq, lambda_eq_run);

  std::function<bool(int,void*)> function_eq = [](int data, void* v) -> bool {
    return data == *static_cast<int*>(v);
  };
#define function_eq_run function_eq(10, &i)
  TEST(function_eq, function_eq_run);

  std::function<bool(void*)> bind_lambda_eq = std::bind(lambda_eq, 10, std::placeholders::_1);
  TEST(bind_lambda_eq, bind_lambda_eq(&i));

  std::function<bool(void*)> bind_function_eq = std::bind(function_eq, 10, std::placeholders::_1);
  TEST(bind_function_eq, bind_function_eq(&i));

  IntEq class_eq(10);
  TEST(class_eq, class_eq.isTrue(&i));

  auto cond = buildCond();
  TEST(virtual_eq, cond->isTrue(&i));
}

main_run;