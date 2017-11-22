#include "common/header.h"

function<int(int)> genVarFunc(int x) {
    return [x] (int v) -> int {
        return x * v;
    };
}

function<int(int)> genRefFunc(int x) {
    return [&x] (int v) -> int {
        return x * v;
    };
}

int main() {
    auto f1 = genVarFunc(1), f2 = genVarFunc(2), f3 = genVarFunc(3);
    printf("%d %d %d\n", f1(1), f2(1), f3(1));
    f1 = genRefFunc(1), f2 = genRefFunc(2), f3 = genRefFunc(3);
    printf("%d %d %d\n", f1(1), f2(1), f3(1));
    return 0;
}