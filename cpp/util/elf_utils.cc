#include <algorithm>
#include <cstring>
#include <iostream>

#include "elf_utils.h"
#include "file_utils.h"
#include "demangle.h"

int getFileElfType(const int fd) {
  ElfW(Ehdr) elf_header;
  if (!readFromOffsetExact(fd, &elf_header, sizeof(elf_header), 0)) {
    return -1;
  }
  if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
    return -1;
  }
  return elf_header.e_type;
}

/**
 * 遍历 $sh_num 个section_header, 找出 $sh_type 的那个section_header
 * 找到返回true, 找不到返回false
*/
bool getSectionHeaderByType(const int fd, ElfW(Half) sh_num, const off_t sh_offset, ElfW(Word) sh_type, ElfW(Shdr)* out) {
  constexpr int sh_size = (int) sizeof(ElfW(Shdr));  // size of section_header
  constexpr int buf_sh_num = 16;    // 一次最多读16个section_header
  ElfW(Shdr) buf[buf_sh_num];
  for (int i = 0, readed_num = 0; i < sh_num; i += readed_num) {
    int left_num = sh_num - i;
    int to_read_num = std::min(buf_sh_num, left_num);
    int len = readFromOffset(fd, buf, to_read_num * sh_size, sh_offset + i * sh_size);
    if (len < sh_size) return false;
    readed_num = len / sh_size;
    for (int j = 0; j < readed_num; ++j) {
      if (buf[j].sh_type == sh_type) {
        *out = buf[j];
        return true;
      }
    }
  }
  return false;  
}

void readAllSymbols(const int fd, const ElfW(Shdr)& symtab, const ElfW(Shdr)& strtab, std::vector<Symbol>* syms) {
  constexpr int sym_size = (int) sizeof(ElfW(Sym));  // size of symbol
  constexpr int buf_sym_num = sizeof(void*) * 32;    // 一次最多读的符号数
  const int sh_size = symtab.sh_entsize;             // size of section_header
  const int sym_num = symtab.sh_size / sh_size;      // 总符号数
  ElfW(Sym) buf[buf_sym_num];
  Symbol sym;
  for (int i = 0, readed_num = 0; i < sym_num; i += readed_num) {
    int to_read_num = std::min(buf_sym_num, sym_num - i);
    int len = readFromOffset(fd, &buf, to_read_num * sym_size, symtab.sh_offset + i * sh_size);
    if (len < sym_size) break;
    readed_num = len / sym_size;
    for (int j = 0; j < readed_num; j++) {
      ElfW(Sym)& symbol = buf[j];
      if (symbol.st_value == 0 || symbol.st_shndx == 0) {
        continue;  // skip null value & undefined symbol
      }
      // st_bind 取值 STB_LOCAL(局部符号 仅在当前文件可见) STB_GLOBAL(全局符号 所有文件可见) STB_WEAK(弱符号 可多个位置重复定义)
      // 弱符号通过 __attribute__((weak)) 指定. 提供默认实现, 可被覆盖
      // int st_bind = (symbol.st_info & 0xF0) >> 4;
      // st_type 取值 STT_OBJECT(变量) STT_FUNC(函数) STT_TLS(threadlocal变量) 等
      int st_type = symbol.st_info & 0x0F;
      if (st_type != STT_FUNC) {
        continue;  // 只保存函数类型的符号
      }
      sym.start_addr = symbol.st_value;
      sym.end_addr = symbol.st_value + symbol.st_size;
      sym.st_name_offset = symbol.st_name + strtab.sh_offset;
      syms->push_back(sym);
      // std::cout << demangleFromFile(fd, sym.st_name_offset) << std::endl;
    }
  }
  std::sort(syms->begin(), syms->end(), [] (const Symbol& x, const Symbol& y) {
    return x.start_addr < y.start_addr;
  });
}

void loadAllSymbols(const int fd, ElfW(Word) sh_type, const ElfW(Ehdr)& elf_header, std::vector<Symbol>* symbols) {
  ElfW(Shdr) symtab, strtab;
  if (!getSectionHeaderByType(fd, elf_header.e_shnum, elf_header.e_shoff, sh_type, &symtab)) {
    return;
  }
  if (!readFromOffsetExact(fd, &strtab, sizeof(ElfW(Shdr)), elf_header.e_shoff + symtab.sh_link * sizeof(ElfW(Shdr)))) {
    return;
  }
  readAllSymbols(fd, symtab, strtab, symbols);
}
