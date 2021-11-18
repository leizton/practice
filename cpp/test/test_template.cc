#include <util/base.h>

struct Temp {
  Temp(void* p_)
      : p(p_) {}

  template <class T>
  T get();

  void* p;
};

template <class T>
T Temp::get() {
  return *static_cast<T*>(p);
}

template <>
std::string Temp::get<std::string>() {
  auto v = *static_cast<double*>(p);
  return std::to_string(v);
}

int main() {
  double v;
  Temp t(&v);

  v = 1.0;
  cout << t.get<double>() << endl;
  cout << std::setbase(16) << t.get<uint64_t>() << endl;
  cout << t.get<std::string>() << endl;

  v = 2.0;
  cout << t.get<double>() << endl;
  cout << std::setbase(16) << t.get<uint64_t>() << endl;
  cout << t.get<std::string>() << endl;
}
