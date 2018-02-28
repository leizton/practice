#include <iostream>
#include "common/header.h"

#define OFFSET(STRUCT,FIELD) reinterpret_cast<uint64_t>(reinterpret_cast<void*>(&reinterpret_cast<STRUCT*>(0)->FIELD))


struct __attribute__(( aligned(8) )) Test1 {
    char a[2];
};

struct __attribute__(( aligned(8) )) Test2 {
    char a[12];
};

struct __attribute__(( aligned(16) )) Test3 {
    char a[2];
};

struct __attribute__(( aligned )) Test4 {
    char a[2];
};


struct __attribute__(()) Test5 {
    char a;
    short b;
};

struct __attribute__(( packed )) Test6 {
    char a;
    short b;
};

struct __attribute__(()) Test7 {
    char a;
    int b;
};

struct __attribute__(( packed )) Test8 {
    char a;
    int b;
};

int main() {
    cout << sizeof(Test1) << endl;  // 8
    cout << sizeof(Test2) << endl;  // 16
    cout << sizeof(Test3) << endl;  // 16
    cout << sizeof(Test4) << endl;  // 16
    cout << endl;

    cout << OFFSET(Test5, b) << endl;  // 2
    cout << OFFSET(Test6, b) << endl;  // 1
    cout << OFFSET(Test7, b) << endl;  // 4
    cout << OFFSET(Test8, b) << endl;  // 1
    cout << sizeof(Test5) << endl;     // 4
    cout << sizeof(Test6) << endl;     // 3
    cout << sizeof(Test7) << endl;     // 8
    cout << sizeof(Test8) << endl;     // 5
    return 0;
}