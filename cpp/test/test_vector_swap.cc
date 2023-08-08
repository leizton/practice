#include <util/base.h>

struct Voo {
  Voo() { v = 0; print("cons"); }
  Voo(int v) { this->v = v; println("cons_v", v); }
  ~Voo() { println("decon", v); }

  Voo(const Voo& e) { v = e.v; println("copy_cons", e.v); }
  Voo(const Voo&& e) { v = e.v; println("rcopy_cons", e.v); }

  Voo& operator=(const Voo& e) { println("operator_eq", e.v, v); v = e.v; return *this; }
  Voo& operator=(const Voo&& e) { println("roperator_eq", e.v, v); v = e.v; return *this; }

  int v;
};

int main() {
  std::vector<Voo> a;
  a.reserve(4);
  a.emplace_back(1);
  a.emplace_back(2);
  std::vector<Voo> b;
  b.reserve(4);
  b.emplace_back(101);
  b.emplace_back(102);
  b.emplace_back(103);
  print("--------------------");
  a.swap(b);
  print("--------------------");
}