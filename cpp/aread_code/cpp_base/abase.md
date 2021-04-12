# INT32_MIN INT32_MAX
```c++
#include <cstdint>
INT8_MIN   INT8_MAX   UINT8_MAX
INT16_MIN  INT16_MAX
INT32_MIN  INT32_MAX
INT64_MIN  INT64_MAX

#include <limits>
numeric_limits<int>::min()
numeric_limits<int>::max()
```


# (函数名 函数指针) 和 (类成员函数 std::function)
## 1 函数名可以隐式转成函数指针
```c++
void func() {}
void (*pf)() = func;
(*pf)();
printf("%p, %p, %p", func, &func, pf);
```
printf打印的3个值是相等的
`pf = func;` 也可以写成 `pf = &func;`;
## 2 类静态成员函数和普通函数没有区别也可以隐式转成函数指针
## 3 类成员函数转成函数指针需要带类名作用域
```c++
struct A {
  void test();
};
typedef void (A::*TestPtr)();
TestPtr test = &A::test;
A a;
(a.*test)();
```
## 4 类成员函数和普通函数有本质上的区别
## 5 为了方便使用, 类成员函数转成std::function
```c++
A a;
std::function<void()> test = std::bind(&A::test, &a);
test();
```


# 类拷贝构造和赋值运算符的区别
```c++
struct A {
  A() : data(nullptr) {}
  void* data;
};
A a;
A a1(a);  // 拷贝构造
A a2 = a; // 拷贝构造, !!!不是赋值运算符!!!
          // 进入拷贝构造时, data是野指针
A a3;     // 默认构造
a3 = a;   // 赋值运算符。进入operator=时, data已被初始化成nullptr
```


# 各种callable的性能比较
[@ref callable_perf](../../stl/functional.cc)


# static_assert(bool_expr, [message])
编译期检查