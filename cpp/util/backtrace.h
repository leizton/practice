#pragma once

#include <bits/stdc++.h>
#include <unwind.h> // unwind build-in gcc

namespace btrace {

class StackTrace;

bool defaultBacktrace(std::string* res);

void unwindBacktrace(StackTrace* record);

/**
 * 获取栈底地址, 即栈帧指针frame
 * @param opaque 透传参数
 */
_Unwind_Reason_Code unwindGetOneFrame(struct _Unwind_Context* uc, void* opaque);

class StackTrace {
public:
  StackTrace(int skip_count = 0)
      : skip_count_(skip_count) {}

  std::string toString();

public:
  void addSkipCount();
  bool trySkip();
  void appendFrame(void* frame);

public:
  std::vector<std::string> stacks;

private:
  int skip_count_;
  std::vector<void*> frames_;
};

} // namespace btrace
