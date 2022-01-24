#include <util/base.h>

class Shape {
public:
  int x = 1;
  virtual void renderA() { print("A-Shape"); }
  virtual void renderB() { print("B-Shape"); }
  virtual ~Shape() {}
};

class Rectangle : public Shape {
public:
  int y = 2;
  void renderA() override { print("A-Rectangle"); }
};

class Square : public Rectangle {
public:
  int z = 3;
  void renderB() override { print("B-Square"); }
};

void callByVptr(Shape* obj) {
  // 指向虚表的指针
  uint64_t vptr_ptr = *(uint64_t*)obj;

  // 虚表: 一个存放实际虚函数地址的数组
  uint64_t vptr = (uint64_t*)vptr_ptr;

  // vptr[0] 即 renderA, 顺序和class里声明顺序相同
  // 保证虚析构函数在最后一个声明
  print(vptr[0], vptr[1]);

  using Fn = void (*)(Shape*);
  auto a = (Fn)vptr[0];
  auto b = (Fn)vptr[1];
  a(obj);
  b(obj);
}

int main() {
  Shape o1;
  Rectangle o2;
  Square o3;
  callByVptr(&o1);
  callByVptr(&o2);
  callByVptr(&o3);
}
