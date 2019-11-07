#pragma once

#include <iostream>
#include <sstream>
#include <string>


#define COUT(expr) std::cout << (expr) << std::endl;

#define FMTSTR_SIZE 1024
#define FMTSTR\
  char fmtstrtmp[FMTSTR_SIZE]; {va_list ap; va_start(ap, fmt);\
  vsnprintf(fmtstrtmp, FMTSTR_SIZE, fmt, ap); va_end(ap);}

void fmtprint(const char* file, const int line, const char* func, const char* fmt, ...) {
  FMTSTR;
  std::ostringstream ss;
  ss << "[" << file << ":" << line << " " << func << "] ";
  ss << fmtstrtmp;
  std::cout << ss.str() << std::endl;
}

#define PRINT(fmt, args...)\
  fmtprint(__FILE__, __LINE__, __FUNCTION__, fmt, ##args);


#define ASSERT_TRUE(expr)\
  if (expr) {}\
  else {\
    PRINT("assert_true error: %s", #expr);\
    exit(1);\
  }
