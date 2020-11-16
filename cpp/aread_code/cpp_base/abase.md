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
## 4 为了方便使用, 类成员函数转成std::function
```c++
A a;
std::function<void()> test = std::bind(&A::test, &a);
test();
```