#include "util/cpp_lib.h"

#define FMTSTR_SIZE 1024
#define FMTSTR\
  char fmtstrtmp[FMTSTR_SIZE]; {va_list ap; va_start(ap, fmt);\
  vsnprintf(fmtstrtmp, FMTSTR_SIZE, fmt, ap); va_end(ap);}

void _printfmt(const char* file, const int line, const char* func, const char* fmt, ...) {
  FMTSTR;
  std::ostringstream ss;
  ss << "[" << file << ":" << line << " " << func << "] ";
  ss << fmtstrtmp;
  std::cout << ss.str() << std::endl;
}

#define printfmt(fmt, args...) _printfmt(__FILE__, __LINE__, __FUNCTION__, fmt, ##args);

#define print(expr) std::cout << std::boolalpha << (expr) << std::endl;

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
ostream& operator <<(ostream& out, const unordered_map<K,V>& m) {
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

template<class T>
void printvec(vector<T> v) {
  ostringstream ss;
  ss << v << "\n";
  cout << ss.str();
}

template<class K, class V>
void printmap(map<K,V> m) {
  ostringstream ss;
  ss << m << "\n";
  cout << ss.str();
}

template<class K, class V>
void printunmap(unordered_map<K,V> m) {
  ostringstream ss;
  ss << m << "\n";
  cout << ss.str();
}
