#pragma once

#include <cstddef>
#include <string>
#include <vector>

#if defined(HAVE_DLFCN_H)
#include <dlfcn.h>  // macos
#endif
#if defined(OS_OPENBSD)
#include <sys/exec_elf.h>
#else
#include <elf.h>
#endif

#if !defined(SIZEOF_VOID_P)
# if defined(__LP64__)
#  define SIZEOF_VOID_P 8
# else
#  define SIZEOF_VOID_P 4
# endif
#endif

#ifndef ElfW
# if SIZEOF_VOID_P == 4
#  define ElfW(type) Elf32_##type
# elif SIZEOF_VOID_P == 8
#  define ElfW(type) Elf64_##type
# else
#  error "Unknown sizeof(void *)"
# endif
#endif

struct Symbol {
  uint64_t start_addr = 0;      // ElfW(Sym).st_value
  uint64_t end_addr = 0;        // ElfW(Sym).st_value + ElfW(Sym).st_size
  uint64_t st_name_offset = 0;  // strtab.sh_offset + ElfW(Sym).st_name
};

int getFileElfType(const int fd);

bool getSectionHeaderByType(const int fd, ElfW(Half) sh_num, const off_t sh_offset, ElfW(Word) sh_type, ElfW(Shdr)* out);

void readAllSymbols(const int fd, const ElfW(Shdr)& symtab, const ElfW(Shdr)& strtab, std::vector<Symbol>* symbols);

void loadAllSymbols(const int fd, ElfW(Word) sh_type, const ElfW(Ehdr)& elf_header, std::vector<Symbol>* symbols);
