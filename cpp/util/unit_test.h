#pragma once

#include "util/cpp_lib.h"

#define OSS std::ostringstream

#define assert_eq(expect, actual) {\
  auto actual_v = actual;\
  _assert_eq(__LINE__, expect, actual_v);\
}

template<class T=const char*, class U=const char*>
void _assert_eq(int lineno, const char* expect, const char* actual) {
  OSS ss;
  ss << "[" << lineno << "] assert_eq";
  if (strcmp(expect, actual) == 0) {
    ss << " ok";
  } else {
    ss << " fail.";
    ss << " expect=\"" << expect << "\", actual=\"" << actual << "\"";
  }
  ss << "\n";
  cout << ss.str();
}

template<class T, class U>
void _assert_eq(int lineno, T expect, U actual) {
  OSS ss;
  ss << "[" << lineno << "] assert_eq";
  if (expect == actual) {
    ss << " ok";
  } else {
    ss << " fail.";
    ss << " expect=" << expect << ", actual=" << actual;
  }
  ss << "\n";
  cout << ss.str();
}

#define assert_T(expr) if (expr) {\
  OSS ss; ss << "[" << __LINE__ << "] assert_T ok\n"; cout << ss.str();\
} else {\
  OSS ss; ss << "[" << __LINE__ << "] assert_T fail\n"; cout << ss.str();\
}

#define assert_F(expr) if (expr) {\
  OSS ss; ss << "[" << __LINE__ << "] assert_F fail\n"; cout << ss.str();\
} else { \
  OSS ss; ss << "[" << __LINE__ << "] assert_F ok\n"; cout << ss.str();\
}
