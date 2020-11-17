# alignof(type)
按几个字节对齐
等于最大的基本数据类型的size
基本数据类型有 bool char short int long_long double void* 等
所以对于64位系统, 可能取值是 1 4 8


# offsetof(type, member)
结构体中数据成员的偏移
偏移量是 alignof(数据成员) 的整数倍


```c++
#include <cstddef>

struct a {
  double a1;
  double a2;
  double a3;
};

/*
  sizeof(A)  == 80
  alignof(A) == 8
*/
struct A {
  char   a1;      // 0
  char   a2[13];  // 1
  int    a3;      // 16
  double a4;      // 24
  a      a5;      // 32
  string a6;      // 56
};

int main() {
  print(sizeof(A));
  print(alignof(A));
  print(offsetof(A, a1));
  print(offsetof(A, a2));
  print(offsetof(A, a3));
  print(offsetof(A, a4));
  print(offsetof(A, a5));
}
```


# 空结构体的情况

```c++
struct A0 {
};

struct A1 {
  char a;
};

struct __attribute__((__aligned__((4)))) A2 {
  char a;
};

int main() {
  print(sizeof(A0), alignof(A0));  // 1, 1
  print(sizeof(A1), alignof(A1));  // 1, 1
  print(sizeof(A2), alignof(A2));  // 4, 4
}
```

__attribute__((__aligned__((4))))
指定按4字节对齐