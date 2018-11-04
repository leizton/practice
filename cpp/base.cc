#include <iostream>

using namespace std;

#define RUN literalNum

// 用户自定义字面值
long long operator"" _k(unsigned long long x) { return x * 1000; }
long long operator"" _K(unsigned long long x) { return (x << 10); }
long double operator"" _k(long double x) { return x * 1000; }
size_t operator"" _len(const char* s, size_t size) { return size; }  // size参数自动推断

void literalNum() {
  cout << 2_k << endl;  // 2000
  cout << 2_K << endl;  // 2048
  cout << 2.6_k << endl;  // 2600
  cout << "123"_len << endl;  // 3
}

int main() {
  RUN();
}