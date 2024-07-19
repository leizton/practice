#pragma once

#include <string>
#include <vector>
#include <unwind.h> // unwind build-in gcc

namespace btrace {

struct StackTrace {
  // 各函数的程序计数器值(Program Counter)
  // pc寄存器存下一条指令地址, 所以也称指令指针(Instruction Pointer, IP)
  std::vector<void*> pcs;
  // pc对应的符号
  std::vector<std::string> syms;
  // 解析符号的错误信息
  int parse_sym_errno = 0;

  std::string toString();

  // 解析符号
  void parseSymbols();
};

void unwindBacktrace(StackTrace* record);

/**
 * 获取函数信息
 * @param opaque 透传参数
 */
_Unwind_Reason_Code unwindGetOneFrame(struct _Unwind_Context* uc, void* opaque);

} // namespace btrace
