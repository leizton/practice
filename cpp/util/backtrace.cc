#include "backtrace.h"

#include <execinfo.h>
#include <fstream>
#include <iostream>

using namespace std;

namespace btrace {

void unwindBacktrace(StackTrace* record) {
  _Unwind_Backtrace(unwindGetOneFrame, record); // 获取调用链每层函数栈底地址(high)
  record->parseSymbols();
}

_Unwind_Reason_Code unwindGetOneFrame(struct _Unwind_Context* uc, void* opaque) {
  auto* record = static_cast<StackTrace*>(opaque);
  void* pc = reinterpret_cast<void*>(_Unwind_GetIP(uc));
  if (pc == nullptr) return _URC_END_OF_STACK;
  record->pcs.push_back(pc);
  return _Unwind_Reason_Code(0);
}

std::string StackTrace::toString() {
  std::string ret;
  ret.reserve(256);
  if (parse_sym_errno != 0) {
    ret += "parse symbols error: " + std::to_string(parse_sym_errno);
  }
  char buf[32];
  int frame_idx = int(pcs.size());
  for (size_t i = 0; i < pcs.size(); i++) {
    frame_idx--;
    snprintf(buf, sizeof(buf) - 1, "%p", pcs[i]);
    ret += "#" + std::to_string(frame_idx) + "  " + std::string(buf);
    if (!syms[i].empty()) {
      ret += " " + syms[i];
    }
    ret += "\n";
  }
  return ret;
}

// 解析符号. @ref glog/symbolize.cc::SymbolizeAndDemangle
void StackTrace::parseSymbols() {
  parse_sym_errno = 0;
  syms.clear();
  if (pcs.empty()) return;
  syms.resize(pcs.size());

  std::ifstream maps_fin("/proc/self/maps");
  if (!maps_fin.is_open()) {
    parse_sym_errno = 1;
    return;
  }
  std::string line;
  while (std::getline(maps_fin, line)) {
    /** 
     * "/proc/self/maps"文件每行格式如下
     * 55cecf1db000-55cecf1de000 r--p 00000000 00:ad 283151112 /opt/cpp/a.out
     * 各字段如下:
     *   - 内存虚拟地址空间的起始-结束地址
     *   - 拥有的读写权限
     *   - 在映射的原文件中的偏移量(单位是页)
     *   - 映射文件所在设备号
     *   - 映射文件所在的inode节点号
     *   - 映射文件名
     */
  }
}

} // namespace btrace
