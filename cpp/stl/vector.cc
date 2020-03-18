#include "common/header.h"

struct Foo {
    int v;

    Foo() : v(0) {
        printf("con default\n");
    }

    Foo(int _v) : v(_v) {
        printf("con    %d\n", _v);
    }

    Foo(const Foo& foo) {
        printf("con &  %d\n", foo.v);
        v = foo.v;
    }

    Foo(const Foo&& foo) {
        printf("con && %d\n", foo.v);
        v = foo.v;
    }

    Foo& operator=(const Foo& foo) {
        printf("con =  %d\n", foo.v);
        if (&foo == this) {
            return *this;
        }
        v = foo.v;
        return *this;
    }

    Foo& operator=(const Foo&& foo) {
        printf("con =  %d\n", foo.v);
        v = foo.v;
        return *this;
    }
};

int main() {
    vector<Foo> foos;
    foos.reserve(3);

    Foo a{-1};  // con -1
    Foo b = a;  // con& -1
    b = a;      // con= -1

    printf("---\n");
    Foo f0{0};
    foos.push_back(f0);  // con 0, con& 0

    printf("---\n");
    foos.push_back(Foo(1));  // con 1, con&& 1

    printf("---\n");
    foos.push_back(2);       // con 2, con&& 2

    // push_back()扩容时调用拷贝构造
    printf("---\n");
    printf("cap    %lu\n",
        foos.capacity());    // 3
    foos.push_back(3);       // con 3, con&& 3, con& 2, con& 1, con& 0
    printf("cap    %lu\n",
        foos.capacity());    // 6

    printf("---\n");
    foos.emplace_back(4);    // con 4

    return 0;
}