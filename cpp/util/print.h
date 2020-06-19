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

void println(int n=1) { while (n-- > 0) std::cout << std::endl; }

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
