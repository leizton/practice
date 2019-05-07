#include <iostream>
#include <stdarg.h>

#include "common/header.h"

void split() {
  string str = "1,2,3";
  stringstream ss(str);
  for (string e; getline(ss, e, ','); ) {
    cout << e << endl;
  }
}

string format(const char* fmt, ...) {
  static const int bufsize = 32;
  char buf[bufsize];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, bufsize, fmt, ap);
  va_end(ap);
  return string(buf);
}

int main() {
  // split();
  // cout << format("test-%d-%s", 17, "abc") << endl;
}