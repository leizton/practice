#include "common/header.h"

struct Foo {
    int v;
    Foo(int x, int y) : v(x * y) { printf("con-Foo: %d\n", v); }
    Foo(int x) : Foo(x, x) {}
    ~Foo() { printf("decon-Foo: %d\n", v); }
};

// 遍历map
void printMap(const unordered_map<int, unique_ptr<Foo>>& map_) {
    printf("{ ");
    for (auto& kv : map_) {
        printf("{%d, %d} ", kv.first, kv.second->v);
    }
    printf("}\n");
}

// 移除map中不符合条件的元素
void filterByKey(unordered_map<int, unique_ptr<Foo>>& map_, function<bool(int)> condition) {
    for (auto it = map_.begin(); it != map_.end(); ) {
        if (condition(it->first)) {
            ++it;
        }
        else {
            map_.erase(it++);
        }
    }
}

int main() {
    unordered_map<int, unique_ptr<Foo>> map_;
    printf(">> init\n");
    printMap(map_);  // {}

    // insert
    printf("\n>> insert\n");
    map_.insert(make_pair<int, unique_ptr<Foo>>(1, make_unique<Foo>(1, 1)));
    printMap(map_);  // { {1, 1} }
    map_[2] = make_unique<Foo>(2);
    printMap(map_);  // { {2, 4} {1, 1} }

    // erase
    printf("\n>> erase\n");
    printf("delete 2: %lu\n", map_.erase(2));  // decon-Foo: 4;  delete 2: 1
    map_[3] = make_unique<Foo>(3);
    map_[4] = make_unique<Foo>(4);
    filterByKey(map_, [](int key) { return key <= 1; });
    printMap(map_);

    // find
    printf("\n>> find\n");
    printf("0 in map: %s\n", map_.find(0) != map_.end() ? "true" : "false");  // false
    printf("1 in map: %s\n", map_.find(1) != map_.end() ? "true" : "false");  // true

    // get
    printf("\n>> get\n");
    {
        unique_ptr<Foo>& ptr = map_[1];
        printf("foo: %d\n", ptr->v);
    }
    printMap(map_);  // { {1, 1} }

    printf("\n>> get not exist\n");
    {
        unique_ptr<Foo>& ptr = map_[2];
        printf("ptr %s null\n", ptr == nullptr ? "is" : "isn't");  // is
    }

    printf("\n");
    return 0;
    // decon-Foo: 1
}