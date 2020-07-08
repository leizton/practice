#include "util/cpp_lib.h"

#define CERR std::cerr << "[" << __FILE__ \
  << ":" << __LINE__ << ", " << __FUNCTION__ << "] "
#define ENDL std::endl

template<class T>
string to_str(T v) {
  ostringstream ss;
  ss << v;
  return ss.str();
}

template<class T>
ostream& operator<<(ostream& out, vector<T> v) {
  out << "[";
  bool first = true;
  for (T& e : v) {
    if (!first) out << ",";
    first = false;
    out << e;
  }
  out << "]";
  return out;
}

template<class K, class V>
ostream& operator<<(ostream& out, map<K,V> m) {
  out << "[";
  bool first = true;
  for (auto& p : m) {
    if (!first) out << ",";
    first = false;
    out << p.first << "=" << p.second;
  }
  out << "]";
  return out;
}

template<class K, class V>
ostream& operator<<(ostream& out, const unordered_map<K,V>& m) {
  out << "[";
  bool first = true;
  for (auto& p : m) {
    if (!first) out << ",";
    first = false;
    out << p.first << "=" << p.second;
  }
  out << "]";
  return out;
}


/**
 * Bufcout
 */
class Bufcout {
public:
  Bufcout() {}
  ostringstream out_;
  constexpr static Bufcout* endl = nullptr;
};

template <class T>
Bufcout&& operator<<(Bufcout&& out, T v) {
  out.out_ << std::boolalpha << v;
  return std::move(out);
}

Bufcout&& operator<<(Bufcout&& out, Bufcout* bc) {
  if (bc != Bufcout::endl) {
    return std::move(out);  // 必须加move, move作用是转成右值引用
  }
  out.out_ << "\n";
  cout << out.out_.str();
  return std::move(out);
}


/**
 * void print(Args... args)
 */
void __print(std::ostringstream& out, bool is_begin) {
}

template<class First, class... Args>
void __print(std::ostringstream& out, bool is_begin, First first, Args... left) {
  if (!is_begin) out << ", ";
  out << first;
  __print(out, false, left...);
}

template<class... Args>
void print(Args... args) {
  std::ostringstream out;
  __print(out, true, args...);
  out << "\n";
  cout << out.str();
}

void println(int n=1) {
  while (n-- > 0) std::cout << std::endl;
}
