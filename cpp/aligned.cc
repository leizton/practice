#include <iostream>
#include "common/header.h"

struct __attribute__(( aligned(8) )) Test1 {
    char a[2];
};

struct __attribute__(( aligned(16) )) Test2 {
    char a[2];
};

struct __attribute__(( aligned )) Test3 {
    char a[2];
};

int main() {
    cout << sizeof(Test1) << endl;  // 8
    cout << sizeof(Test2) << endl;  // 16
    cout << sizeof(Test3) << endl;  // 16
    return 0;
}