#include <iostream>
using namespace std;

void func() {
    static int g = 0;
    int i = 0, j = 1;
    cout << "] f-" << (uint64_t) func
         << ", g-" << (uint64_t) &g
         << ", i-" << (uint64_t) &i
         << ", j-" << (uint64_t) &j - (uint64_t) &i
         << endl;
}

void func1() {
    func();
}

int main() {
    func();
    func();   // i的地址和上次相同
    func1();  // i的地址和之前不同
    func();   // i的地址和第1/2次相同
    return 0;
}