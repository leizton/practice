#pragma once

#include "print.h"

class LogStream {
 public:
  LogStream() {
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
  std::ostringstream sout_;
};

#define LOG() LogStream()
#define LOG_INFO() LogStream(__FILE__, __LINE__)
