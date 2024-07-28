#include <unistd.h>
#include <errno.h>

#include "file_utils.h"

int readFromOffset(const int fd, void* _buf, const size_t count, const off_t offset) {
  char* buf = reinterpret_cast<char*>(_buf);
  int num_bytes = 0;
  while (num_bytes < count) {
    int len;
    do {
      len = ::pread(fd, buf + num_bytes, count - num_bytes, offset + num_bytes);
    } while (len < 0 && errno == EINTR);  // EINTR时重试, 即系统调用被中断后重试
    if (len < 0) return -1;
    if (len == 0) break;  // reach EOF
    num_bytes += len;
  }
  return num_bytes;
}

void FileFd::close_fd() {
  if (is_open()) {
    ::close(fd);
  }
  const_cast<int&>(fd) = -1;
}

FileFd openFile(const char* file, int flag) {
  int fd = ::open(file, flag);
  return FileFd(fd);
}
