#include "common/header.h"

struct Foo {
    int v;

    Foo(int _v) : v(_v) {
        printf("con %d\n", _v);
    }

    Foo(const Foo&& foo) {
        printf("con&& %d\n", foo.v);
        v = foo.v;
    }

    Foo(const Foo& foo) {
        printf("con& %d\n", foo.v);
        v = foo.v;
    }
};

int main() {
    vector<Foo> foos;
    foos.reserve(4);
    foos.push_back(Foo(1));  // con con&&
    foos.push_back(2);  // con con&&
    return 0;
}