# 头文件里不要写函数的实现
.cc include头文件时是内容直接复制进来，所以如果.h里包含非inline函数的实现，当多个.cc include他时会出现重复定义的编译错误
class头文件的函数默认是inline，所以写实现不会报重复定义
https://blog.csdn.net/aiaiai010101/article/details/78178086


# static不需要出现在.cc，const需要
```cpp
// aoo.h
class Aoo {
public:
  int get() const;
private:
  int a_;
  static const int num;
};
// aoo.cpp
const int Aoo::num = 10;
int Aoo::get() const { return a_; }
```


# 类的const成员变量
```cpp
class Aoo {
public:
  Aoo(int a) : a_(a) {}
private:
  const int a_;
};
```