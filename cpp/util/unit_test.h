#pragma once

#include "util/cpp_lib.h"

#define OSS std::ostringstream

namespace unit_test {
bool AssertLog = false;
}

#define assert_T(expr) if (expr) {\
  if (unit_test::AssertLog) {OSS ss; ss << "[" << __LINE__ << "] assert_T ok\n"; cout << ss.str();}\
} else {\
  OSS ss; ss << "[" << __LINE__ << "] assert_T fail\n"; cout << ss.str();\
}

#define assert_F(expr) if (expr) {\
  OSS ss; ss << "[" << __LINE__ << "] assert_F fail\n"; cout << ss.str();\
} else { \
  if (unit_test::AssertLog) {OSS ss; ss << "[" << __LINE__ << "] assert_F ok\n"; cout << ss.str();}\
}

#define assert_eq(expect, actual) {\
  auto actual_v = actual;\
  _assert_eq(__LINE__, expect, actual_v);\
}

template<class T, class U>
void _assert_eq(int lineno, T expect, U actual) {
  bool log = unit_test::AssertLog;
  OSS ss;
  ss << "[" << lineno << "] assert_eq";
  if (expect == actual) {
    ss << " ok";
  } else {
    log = true;
    ss << " fail.";
    ss << " expect=" << expect << ", actual=" << actual;
  }
  ss << "\n";
  if (log) cout << ss.str();
}

template<>
void _assert_eq(int lineno, const char* expect, const char* actual) {
  bool log = unit_test::AssertLog;
  OSS ss;
  ss << "[" << lineno << "] assert_eq";
  if (strcmp(expect, actual) == 0) {
    ss << " ok";
  } else {
    log = true;
    ss << " fail.";
    ss << " expect=\"" << expect << "\", actual=\"" << actual << "\"";
  }
  ss << "\n";
  if (log) cout << ss.str();
}

#define assert_neq(expect, actual) {\
  auto actual_v = actual;\
  _assert_neq(__LINE__, expect, actual_v);\
}

template<class T=const char*, class U=const char*>
void _assert_neq(int lineno, const char* expect, const char* actual) {
  bool log = unit_test::AssertLog;
  OSS ss;
  ss << "[" << lineno << "] assert_neq";
  if (strcmp(expect, actual) == 0) {
    log = true;
    ss << " fail.";
  } else {
    ss << " ok.";
  }
  ss << " expect=\"" << expect << "\", actual=\"" << actual << "\"";
  ss << "\n";
  if (log) cout << ss.str();
}

template<class T, class U>
void _assert_neq(int lineno, T expect, U actual) {
  bool log = unit_test::AssertLog;
  OSS ss;
  ss << "[" << lineno << "] assert_neq";
  if (expect == actual) {
    log = true;
    ss << " fail.";
  } else {
    ss << " ok.";
  }
  ss << " expect=" << expect << ", actual=" << actual;
  ss << "\n";
  if (log) cout << ss.str();
}
