# static_cast
  implicit cast like `int to float`, `a pointer to void*`
  explicit cast
  继承关系中, 没有virtual时, 向下转型


# const_cast
  remove or add `const` to a variable


# dynamic_cast
  is exclusively(only) used for handling polymorphism.
  a polymorphic type has at least one virtual function, declared or inherited.
  向下转型失败时, return `nullptr` in the case of a pointer, or throw `std::bad_cast` in the case of a reference
example
```c++
  // B C 虚继承 A, 形成菱形继承关系
  struct A { virtual int get() { return 1; } };
  struct B : virtual public A { int get() override { return 2; } };
  struct C : virtual public A { int get() override { return 3; } };
  struct D : public B, public C { int get() override { return 4; } };
  int main() {
    std::shared_ptr<A> a = std::make_shared<D>();
    a->get(); // 4
    C* c = dynamic_cast<C*>(a.get());
    c->get(); // 4
    std::shared_ptr<D> d = std::dynamic_pointer_cast<D>(a);
    d->get(); // 4
  }
```


# reinterpret_cast
  the most dangerous cast.
  cast a pointer to another, or storing a pointer in an uint64_t