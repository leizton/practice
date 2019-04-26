#include <iostream>
#include <memory>

using namespace std;


struct Foo {
  Foo() : v_(0) { cout << "Foo-con v=0\n"; }
  Foo(int v) : v_(v) { cout << "Foo-con v=" << v << "\n"; }

  virtual ~Foo() { cout << "Foo-decon\n"; }

  virtual void print() {
    cout << "Foo{vP,v=" << v_ << "}\n";
  }

  void nonVirtualPrint() {
    cout << "Foo{nvP,v=" << v_ << "}\n";
  }

  int v_;
};

struct A : public Foo {
  A() : a_(0) { cout << "A-con a=0\n"; }
  A(int v, int a) : Foo(v), a_(a) {
    cout << "A-con v=" << v << " a=" << a << "\n";
  }

  ~A() { cout << "A-decon\n"; }

  void print() {
    cout << "A{vP,v=" << v_ << ",a=" << a_ << "}\n";
  }

  void nonVirtualPrint() {
    cout << "A{nvP,v=" << v_ << ",a=" << a_ << "}\n";
  }

  int a_;
};


// #define RUN base
// make_shared<T>(Args&&... args)
// shared_ptr的copy-assign和assign
// 自身赋值
void base() {
  auto p1 = make_shared<Foo>(10);
  auto p2 = p1;
  auto p3(p2);
  cout << p1.use_count();  // 3

  {
    auto p4 = p3;
    cout << p1.use_count(); // 4
  }
  cout << p1.use_count(); // 3

  p1 = p1;  // operator= 里判断 this== &another
  cout << p1.use_count();  // 3
}

// 管理资源. 利用析构函数释放资源
void manageRes() {
  int v = 0;
  shared_ptr<void> ptr(nullptr, [&v](void* p) {
    cout << "release v=" << v;
  });

  v += 100;
}


#define RUN testBasePtrToDerivedPtr
// std::dynamic_pointer_cast<T>(base_ptr)
void testBasePtrToDerivedPtr() {
  auto base_ptr = make_shared<A>(3, 5);
  base_ptr->print();             // A
  base_ptr->nonVirtualPrint();   // A

  shared_ptr<A> ptr = std::dynamic_pointer_cast<A>(base_ptr);
  if (ptr == nullptr) {
    cout << "error\b";
    return;
  }

  ptr->v_ *= 2;
  ptr->a_ *= ptr->a_;
  ptr->print();                  // A
  ptr->nonVirtualPrint();        // A

  shared_ptr<Foo> base_ptr1 = std::dynamic_pointer_cast<Foo>(ptr);
  base_ptr1->print();            // A
  base_ptr1->nonVirtualPrint();  // Foo
}


int main() {
  RUN();
}
