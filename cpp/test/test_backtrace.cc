#include "util/base.h"

void printStackTrace() {
  btrace::StackTrace record;
  btrace::unwindBacktrace(&record);
  LOG() << record.toString();
}

void run_c(int x) {
  LOG() << "-------------------- c()";
  printStackTrace();
  std::vector<int> v;
  v.at(10) = x;
  LOG() << "-------------------- c() ret";
}

void run_b() {
  LOG() << "-------------------- b()";
  run_c(1);
  LOG() << "-------------------- b() ret";
}

void run_a() {
  LOG() << "-------------------- a()";
  try {
    run_b();
  } catch (exception& ex) {
    LOG() << "-------------------- a() catch";
    printStackTrace();
  }
  LOG() << "-------------------- a() ret";
}

int main() {
  run_a();
  return 0;
}
