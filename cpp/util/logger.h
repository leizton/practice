#pragma once

#include "print.h"

class LogStream {
 public:
  LogStream() {
  }
  LogStream(int line) {
    sout_ << "[" << line << "] ";
  }
  LogStream(const char* file, int line) {
    sout_ << "[" << file << ":" << line << "] ";
  }

  ~LogStream() {
    sout_ << "\n";
    std::cout << sout_.str();
  }

  template <class T>
  LogStream& operator<<(const T& t) {
    sout_ << t;
    return *this;
  }

  LogStream& write(const char* s, int len) {
    sout_.write(s, len);
    return *this;
  }

 private:
  std::ostringstream sout_; // TODO: 替换. std::ostringstream 在多线程场景有锁性能问题
};

#define LOG() LogStream()
#define LOG_LINE() LogStream(__LINE__)
#define LOG_INFO() LogStream(__FILE__, __LINE__)
