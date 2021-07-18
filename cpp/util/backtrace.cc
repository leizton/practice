#include <execinfo.h>

#include "util/backtrace.h"

namespace btrace {

bool defaultBacktrace(std::string* res) {
  static const int kStackLength = 64;
  void* stack[kStackLength];
  int num = backtrace(stack, kStackLength);
  char** ss = backtrace_symbols(stack, num);
  if (ss == nullptr) {
    return false;
  }
  for (int i = 1; i < num; i++) {
    *res += std::string(ss[i]) + "\n";
  }
  free(ss);
  return true;
}

void unwindBacktrace(StackTrace* record) {
  record->addSkipCount();                       // 跳过当前栈
  _Unwind_Backtrace(unwindGetOneFrame, record); // 获取函数地址

  // @ref glog/symbolize.cc::SymbolizeAndDemangle 解析符号
}

_Unwind_Reason_Code unwindGetOneFrame(struct _Unwind_Context* uc, void* opaque) {
  auto* record = static_cast<StackTrace*>(opaque);
  if (record->trySkip()) return _URC_NO_REASON;

  void* frame = reinterpret_cast<void*>(_Unwind_GetIP(uc));
  if (frame == nullptr) return _URC_END_OF_STACK;

  record->appendFrame(frame);
  return _URC_NO_REASON;
}

std::string StackTrace::toString() {
  std::string ret;
  char buf[1024];
  for (size_t i = 0; i < frames_.size(); i++) {
    snprintf(buf, sizeof(buf) - 1, "%p", frames_[i]);
    ret += "#" + std::to_string(i) + "  " + std::string(buf) + "\n";
  }
  return ret;
}

void StackTrace::addSkipCount() {
  skip_count_++;
}

bool StackTrace::trySkip() {
  if (skip_count_ > 0) {
    skip_count_--;
    return true;
  } else {
    return false;
  }
}

void StackTrace::appendFrame(void* frame) {
  frames_.push_back(frame);
}

} // namespace btrace