#include <iostream>
#include <mutex>
#include <sys/mman.h>

char* mem_base() {
  static char* known;
  static std::once_flag flag;
  std::call_once(flag, [] {
    const size_t alloc = (size_t) 1 << 44;
    void* r = ::mmap(NULL, (alloc << 1), PROT_NONE,
      MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE,
      -1, 0);
    if (r == (void*) -1) {
      abort();
    }
    std::cout << "r: " << reinterpret_cast<size_t>(r) << std::endl;
    ::madvise(r, (alloc << 1), MADV_DONTDUMP);
    size_t v = reinterpret_cast<size_t>(r);
    size_t k = (v + alloc - 1) & ~(alloc - 1);
    ::munmap(r, k - v);
    std::cout << "k: " << k << std::endl;
    known = reinterpret_cast<char*>(k);
    ::munmap(known + alloc,     
      (reinterpret_cast<char*>(r) + (alloc << 1)) - (known + alloc)
    );
  });
  return known;
}

int main() {
  mem_base();
}