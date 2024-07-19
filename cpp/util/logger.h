#pragma once

#include <sstream>
#include <iostream>

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
    std::cout << sout_.str() << std::endl;
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
#define LOG_LINE() LogStream(__LINE__)
#define LOG_INFO() LogStream(__FILE__, __LINE__)
#define LOG_ERROR() LogStream(__FILE__, __LINE__)
