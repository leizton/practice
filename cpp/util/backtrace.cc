#include <execinfo.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

#include "backtrace.h"
#include "elf_utils.h"
#include "demangle.h"
#include "file_utils.h"

using namespace std;

namespace btrace {

struct BtraceInfo {
  std::string exe_filepath;
  std::vector<Symbol> static_symbols;
  std::vector<Symbol> dynamic_symbols;

  std::string getSymbol(const int exe_fd, uint64_t pc) const;

  static const Symbol* findSymbol(const std::vector<Symbol>& sorted_symbols, uint64_t pc);
};

static BtraceInfo btrace_info;

// 获取函数信息. 透传参数 opaque
_Unwind_Reason_Code unwindGetOneFrame(struct _Unwind_Context* uc, void* opaque) {
  auto* record = static_cast<StackTrace*>(opaque);
  void* pc = reinterpret_cast<void*>(_Unwind_GetIP(uc));
  if (pc == nullptr) return _URC_END_OF_STACK;
  if (record->skip_stack > 0) {
    --record->skip_stack;
  } else {
    record->pcs.push_back(pc);
  }
  return _Unwind_Reason_Code(0);
}

void unwindBacktrace(StackTrace* record) {
  record->skip_stack = 1;
  _Unwind_Backtrace(unwindGetOneFrame, record); // 获取调用链每层函数栈底地址(high)
  record->parseSymbols();
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

struct pc_info {
  int ori_idx = -1;
  uint64_t pc = 0;  // 内存中的绝对地址
  uint64_t pc_relative = 0;  // 相对地址
};

int binarySearchLower(const std::vector<pc_info>& pcs, uint64_t addr) {
  int l = 0, h = (int)pcs.size();
  while (l < h) {
    int mid = (l + h) >> 1;
    if (pcs[mid].pc >= addr) h = mid;
    else l = mid + 1;
  }
  return l;
}

int binarySearchUpper(const std::vector<pc_info>& pcs, uint64_t addr) {
  int l = 0, h = (int)pcs.size();
  while (l < h) {
    int mid = (l + h) >> 1;
    if (pcs[mid].pc > addr) h = mid;
    else l = mid + 1;
  }
  return l;
}

// 解析符号. @ref glog/src/symbolize.cc::SymbolizeAndDemangle
// @ref glog/src/utilities.cc::DumpStackTraceAndExit->DumpPCAndSymbol->SymbolizeAndDemangle; LOG(FATAL) abort退出前打印栈信息
// 接着用 addr2line 获取pc所在代码文件和行号
bool StackTrace::parseSymbols() {
  #define PARSE_ERROR_RETURN(err) { parse_sym_errno = err; return false; }
  parse_sym_errno = 0;
  syms.clear();
  if (pcs.empty()) return true;
  syms.resize(pcs.size());

  if (btrace_info.exe_filepath.empty()) PARSE_ERROR_RETURN(1);
  if (btrace_info.static_symbols.empty() && btrace_info.dynamic_symbols.empty()) PARSE_ERROR_RETURN(2);

  const int pc_num = (int)pcs.size();
  std::vector<pc_info> pc_infos(pcs.size());
  for (int i = 0; i < pc_num; i++) {
    pc_infos[i].ori_idx = i;
    pc_infos[i].pc = reinterpret_cast<uint64_t>(pcs[i]);
  }
  std::sort(pc_infos.begin(), pc_infos.end(), [] (const pc_info& x, const pc_info& y) {
    return x.pc < y.pc;
  });

  FileFd mem_fd(open("/proc/self/mem", O_RDONLY));
  if (!mem_fd.is_open()) PARSE_ERROR_RETURN(3);

  std::ifstream maps_fin("/proc/self/maps");
  if (!maps_fin.is_open()) PARSE_ERROR_RETURN(4);
  std::string line;
  uint64_t base_addr = 0;
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
    {
      auto& tmp = btrace_info.exe_filepath;
      size_t n1 = tmp.length(), n2 = line.length();
      if (n1 > n2 || strncmp(line.c_str() + (n2 - n1), tmp.c_str(), n) != 0) {
        continue;  // 只解析本binary的符号, 不解析动态库so的符号
      }
    }      

    // start_addr end_addr
    const char* p = line.c_str();
    char* pend = nullptr;
    uint64_t start_addr = std::strtoull(p, &pend, 16);
    if (pend == nullptr || *pend != '-') PARSE_ERROR_RETURN(5);
    p = ++pend;
    pend = nullptr;
    uint64_t end_addr = std::strtoull(p, &pend, 16);
    if (pend == nullptr || *pend != ' ') PARSE_ERROR_RETURN(6);
    p = ++pend;

    // base_addr. 每次都解析
    ElfW(Ehdr) ehdr;
    if (readFromOffsetExact(mem_fd.fd, &ehdr, sizeof(ehdr), start_addr) &&
        memcmp(ehdr.e_ident, ELFMAG, SELFMAG) == 0) {
      if (ehdr.e_type == ET_EXEC) {
        base_addr = 0;
      } else if (ehdr.e_type == ET_DYN) {
        base_addr = start_addr;
        for (unsigned i = 0; i != ehdr.e_phnum; ++i) {
          ElfW(Phdr) phdr;
          if (readFromOffsetExact(mem_fd.fd, &phdr, sizeof(phdr), start_addr + ehdr.e_phoff + i*sizeof(phdr)) &&
              phdr.p_type == PT_LOAD && phdr.p_offset == 0) {
            base_addr = start_addr - phdr.p_vaddr;
            break;
          }
        }
      }
    }

    // only readable maps
    if (*p != 'r') continue;

    if (start_addr > pc_infos[pc_num-1].pc) break;
    if (end_addr <= pc_infos[0].pc) continue;
    int low = binarySearchLower(pc_infos, start_addr);
    int high = binarySearchUpper(pc_infos, end_addr - 1);
    high = std::min(high, pc_num);
    for (int i = low; i < high; i++) {
      pc_infos[i].pc_relative = pc_infos[i].pc - base_addr;
    }
  }
  maps_fin.close();
  mem_fd.close_fd();

  auto exe_fd = openFile(btrace_info.exe_filepath.c_str(), O_RDONLY);
  if (!exe_fd.is_open()) PARSE_ERROR_RETURN(7);
  for (auto& pc_info : pc_infos) {
    syms[pc_info.ori_idx] = btrace_info.getSymbol(exe_fd.fd, pc_info.pc_relative);
  }

  #undef PARSE_ERROR_RETURN
  return true;
}

void init(std::string exe_filepath) {
  if (exe_filepath.empty()) {
    char buf[1024];
    int exe_filepath_size = readlink("/proc/self/exe", buf, 1024);
    if (exe_filepath_size == -1) {
      std::cerr << "[backtrace] readlink fail" << std::endl;
      return;
    }
    btrace_info.exe_filepath = std::string(buf, exe_filepath_size);
  } else {
    btrace_info.exe_filepath = exe_filepath;
  }
  std::cout << "[backtrace] exe_filepath: " << btrace_info.exe_filepath << std::endl;

  auto exe_fd = openFile(btrace_info.exe_filepath.c_str(), O_RDONLY);
  if (!exe_fd.is_open()) {
    std::cout << "[backtrace] open exe_file fail" << std::endl;
    return;
  }
  int elf_type = getFileElfType(exe_fd.fd);
  if (elf_type == -1) {
    std::cout << "[backtrace] get elf_type fail" << std::endl;
    return;
  }
  ElfW(Ehdr) elf_header;
  if (!readFromOffsetExact(exe_fd.fd, &elf_header, sizeof(elf_header), 0)) {
    std::cout << "[backtrace] read elf_header fail" << std::endl;
    return;
  }

  // 静态符号表
  loadAllSymbols(exe_fd.fd, SHT_SYMTAB, elf_header, &btrace_info.static_symbols);
  // 动态符号表
  loadAllSymbols(exe_fd.fd, SHT_DYNSYM, elf_header, &btrace_info.dynamic_symbols);
  std::cout << "[backtrace] static_symbols num: " << btrace_info.static_symbols.size() << std::endl;
  std::cout << "[backtrace] dynamic_symbols num: " << btrace_info.dynamic_symbols.size() << std::endl;
}

std::string BtraceInfo::getSymbol(const int exe_fd, uint64_t pc) const {
  auto sym = findSymbol(static_symbols, pc);
  if (sym == nullptr) {
    sym = findSymbol(dynamic_symbols, pc);
  }
  if (sym == nullptr) {
    return "";
  }
  return demangleFromFile(exe_fd, sym->st_name_offset);
}

const Symbol* BtraceInfo::findSymbol(const std::vector<Symbol>& sorted_symbols, uint64_t pc) {
  if (sorted_symbols.empty() ||
      pc < sorted_symbols[0].start_addr ||
      pc >= sorted_symbols.back().end_addr) {
    return nullptr;
  }
  int l = 0, h = (int)sorted_symbols.size();
  while (l < h) {
    int mid = (l + h) >> 1;
    if (sorted_symbols[mid].start_addr > pc) {
      h = mid;
    } else if (sorted_symbols[mid].start_addr == pc) {
      return &sorted_symbols[mid];
    } else if (sorted_symbols[mid].end_addr > pc) {
      return &sorted_symbols[mid];
    } else {
      l = mid + 1;
    }
  }
  return nullptr;
}

} // namespace btrace
