#include "common/header.h"

struct Foo {
    int v;
    Foo(int x, int y) : v(x * y) { printf("con-Foo: %d\n", v); }
    Foo(int x) : Foo(x, x) {}
    ~Foo() { printf("decon-Foo: %d\n", v); }
};

void printMap(const unordered_map<int, unique_ptr<Foo>>& map_) {
    printf("{ ");
    for (auto& kv : map_) {
        printf("{%d, %d} ", kv.first, kv.second->v);
    }
    printf("}\n");
}

int main() {
    unordered_map<int, unique_ptr<Foo>> map_;
    printMap(map_);

    map_.insert(make_pair<int, unique_ptr<Foo>>(1, make_unique<Foo>(1, 1)));
    printMap(map_);

    map_[2] = make_unique<Foo>(2);
    printMap(map_);

    printf("delete 2: %lu\n", map_.erase(2));

    printf("0 in map: %s\n", map_.find(0) != map_.end() ? "true" : "false");
    printf("1 in map: %s\n", map_.find(1) != map_.end() ? "true" : "false");

    {
        unique_ptr<Foo>& ptr = map_[1];
        printf("foo: %d\n", ptr->v);
    }
    printMap(map_);

    {
        unique_ptr<Foo>& ptr = map_[2];
        printf("ptr %s null\n", ptr == nullptr ? "is" : "isn't");
    }

    return 0;
}