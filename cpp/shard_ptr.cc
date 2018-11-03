#include <iostream>
#include <memory>

using namespace std;

#define RUN base

struct Foo;

// make_shared<T>(Args&&... args)
// shared_ptr的copy-assign和assign
// 自身赋值
void base() {
  auto p1 = make_shared<Foo>(10);
  auto p2 = p1;
  auto p3(p2);
  cout << p1.use_count() << endl;  // 3

  {
    auto p4 = p3;
    cout << p1.use_count() << endl; // 4
  }
  cout << p1.use_count() << endl; // 3

  p1 = p1;  // operator= 里判断 this== &another
  cout << p1.use_count() << endl;  // 3
}

// 管理资源. 利用析构函数释放资源
void manageRes() {
  int v = 0;
  shared_ptr<void> ptr(nullptr, [&v](void* p) {
    cout << "release v=" << v << endl;
  });

  v += 100;
}

int main() {
  RUN();
}

struct Foo {
  Foo() : v_(0) { cout << "Foo-con v=0" << endl; }
  Foo(int v) : v_(v) { cout << "Foo-con v=" << v << endl; }
  ~Foo() { cout << "Foo-decon" << endl; }

  int v_;
};