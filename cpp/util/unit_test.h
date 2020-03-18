#pragma once

#include "util/cpp_lib.h"

#define assert_eq(expect, actual) {\
  auto actual_v = actual;\
  _assert_eq(__LINE__, expect, actual_v);\
}

template<class T, class U>
void _assert_eq(int lineno, T expect, U actual) {
  ostringstream ss;
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

#define assert_T(expr) if (expr) {} else {\
  ostringstream ss;\
  ss << "[" << __LINE__ << "] assert_T fail\n";\
  cout << ss.str();\
}

#define assert_F(expr) if (expr) {\
  ostringstream ss;\
  ss << "[" << __LINE__ << "] assert_F fail\n";\
  cout << ss.str();\
}
