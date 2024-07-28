#include "util/backtrace.h"
#include "util/logger.h"

void printStackTrace() {
  btrace::StackTrace record;
  btrace::unwindBacktrace(&record);
  LOG() << record.toString();
}

void run_c(int x) {
  LOG() << "-------------------- c() entry";
  printStackTrace();
  std::vector<int> v;
  v.at(10) = x;  // throw exception
  LOG() << "-------------------- c() return";
}

void run_b() {
  LOG() << "-------------------- b() entry";
  run_c(1);
  LOG() << "-------------------- b() return";
}

void run_a() {
  LOG() << "-------------------- a() entry";
  try {
    run_b();
  } catch (...) {
    LOG() << "-------------------- a() catch exception";
    printStackTrace();
  }
  LOG() << "-------------------- a() return";
}

int main() {
  btrace::init();
  run_a();
  return 0;
}
