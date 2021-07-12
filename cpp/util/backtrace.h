#pragma once

#include <execinfo.h>
#include <string>
#include <unwind.h> // unwind build-in gcc
#include <vector>

#include "logger.h"

struct StackTraceRecord {
  StackTraceRecord(int skip_count = 0)
      : skip_count_(skip_count) {}

  std::string toString() {
    std::string ret;
    char buf[1024];
    for (size_t i = 0; i < stack_.size(); i++) {
      snprintf(buf, sizeof(buf) - 1, "%p", stack_[i]);
      ret += "#" + std::to_string(i) + "  " + std::string(buf) + "\n";
    }
    return ret;
  }

public:
  void addSkipCount() {
    skip_count_++;
  }

  bool trySkip() {
    if (skip_count_ > 0) {
      skip_count_--;
      return true;
    } else {
      return false;
    }
  }

  void append(void* frame) {
    stack_.push_back(frame);
  }

private:
  int skip_count_;
  std::vector<void*> stack_;
};

inline _Unwind_Reason_Code unwindGetOneFrame(struct _Unwind_Context* uc, void* opaque) {
  auto* record = static_cast<StackTraceRecord*>(opaque);
  if (record->trySkip()) return _URC_NO_REASON;

  void* frame = reinterpret_cast<void*>(_Unwind_GetIP(uc));
  if (frame == nullptr) return _URC_END_OF_STACK;

  record->append(frame);
  return _URC_NO_REASON;
}

inline void backtraceByUnwind(StackTraceRecord* record) {
  record->addSkipCount(); // 跳过当前栈
  _Unwind_Backtrace(unwindGetOneFrame, record); // 获取函数地址

  // @ref glog/symbolize.cc::SymbolizeAndDemangle 解析符号
}

inline void backtrace() {
  static const int kStackLength = 64;
  void* stack[kStackLength];
  int num = backtrace(stack, kStackLength);
  char** ss = backtrace_symbols(stack, num);
  if (ss != nullptr) {
    for (int i = 1; i < num; i++) {
      LOG() << ss[i];
    }
    free(ss);
  } else {
    LOG() << "get stack trace fail";
  }
}
