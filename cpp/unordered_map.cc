#include "common/header.h"

struct Foo {
    int v;
    Foo(int x, int y) : v(x * y) { printf("con-Foo: %d\n", v); }
    Foo(int x) : Foo(x, x) {}
    ~Foo() { printf("decon-Foo: %d\n", v); }
};

// 遍历 map
void printMap(const unordered_map<int, unique_ptr<Foo>>& map_) {
    printf("{ ");
    for (auto& kv : map_) {
        printf("{%d, %d} ", kv.first, kv.second->v);
    }
    printf("}\n");
}

int main() {
    unordered_map<int, unique_ptr<Foo>> map_;
    printMap(map_);  // {}

    // insert
    map_.insert(make_pair<int, unique_ptr<Foo>>(1, make_unique<Foo>(1, 1)));
    printMap(map_);  // { {1, 1} }
    map_[2] = make_unique<Foo>(2);
    printMap(map_);  // { {2, 4} {1, 1} }

    // erase
    printf("delete 2: %lu\n", map_.erase(2));  // decon-Foo: 4;  delete 2: 1

    // find
    printf("0 in map: %s\n", map_.find(0) != map_.end() ? "true" : "false");  // false
    printf("1 in map: %s\n", map_.find(1) != map_.end() ? "true" : "false");  // true

    // get
    {
        unique_ptr<Foo>& ptr = map_[1];
        printf("foo: %d\n", ptr->v);
    }
    printMap(map_);  // { {1, 1} }

    {
        unique_ptr<Foo>& ptr = map_[2];
        printf("ptr %s null\n", ptr == nullptr ? "is" : "isn't");  // is
    }

    return 0;
    // decon-Foo: 1
}