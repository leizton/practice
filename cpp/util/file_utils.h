#pragma once

#include <fcntl.h>

int readFromOffset(const int fd, void* buf, const size_t count, const off_t offset);

inline bool readFromOffsetExact(const int fd, void* buf, const size_t count, const off_t offset) {
  return int(readFromOffset(fd, buf, count, offset)) == count;
}

struct FileFd {
  const int fd = -1;

  FileFd(int _fd) : fd(_fd) {}
  ~FileFd() { close_fd(); }
  bool is_open() const { return fd >= 0; }
  void close_fd();
};

FileFd openFile(const char* file, int flag);
