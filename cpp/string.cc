#include <iostream>

#include "common/header.h"

void split() {
  string str = "1,2,3";
  stringstream ss(str);
  for (string e; getline(ss, e, ','); ) {
    cout << e << endl;
  }
}

int main() {
  split();
}