#pragma once

#include <bits/stdc++.h>

template <class T>
void printHex(T x) {
  std::cout << std::setbase(16) << x << std::endl;
}

template <class T>
void printBin(T x) {
  std::cout << std::setbase(2) << x << std::endl;
}

template <class K, class V>
std::ostream& operator<<(std::ostream& out, std::pair<K, V> p) {
  out << "(" << p.first << "," << p.second << ")";
  return out;
}

template <class Container>
std::ostream& print_seq_container(std::ostream& out, const Container& c) {
  out << "[";
  for (const auto& e : c) {
    out << e << ",";
  }
  out.seekp(out.tellp() - 1);
  out << "]";
  return out;
}

template <class Container>
std::ostream& print_map_container(std::ostream& out, const Container& c) {
  out << "[";
  for (const auto& p : c) {
    out << p.first << "=" << p.second << ",";
  }
  out.seekp(out.tellp() - 1);
  out << "]";
  return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  return print_seq_container(out, v);
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& l) {
  return print_seq_container(out, l);
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& s) {
  return print_seq_container(out, s);
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::unordered_set<T>& s) {
  return print_seq_container(out, s);
}

template <class K, class V>
std::ostream& operator<<(std::ostream& out, const std::map<K, V>& m) {
  return print_map_container(out, m);
}

template <class K, class V>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<K, V>& m) {
  return print_map_container(out, m);
}

inline void __out_to_stream(std::ostream& out, bool v) {
  out << std::boolalpha << v;
}

inline void __out_to_stream(std::ostream& out, double d) {
  static int precision = 4;
  out << std::to_string(int64_t(d));
  if (d != (int64_t)d) {
    auto s1 = std::to_string(d - (int64_t)d);
    out << s1.substr(1, std::min(precision + 1, (int)s1.find_last_not_of('0')));
  }
}

template <class T>
void __out_to_stream(std::ostream& out, const T& v) {
  out << v;
}

template <class T>
std::string to_str(const T& v) {
  std::ostringstream ss;
  out_to_stream(ss, v);
  return ss.str();
}

/**
 * void print(Args... args)
 */
inline void __print(std::ostringstream& out, std::string sep, bool is_begin) {
}

template <class First, class... Args>
void __print(std::ostringstream& out, std::string sep, bool non_first, const First& first, const Args&... left) {
  if (non_first) out << sep;
  __out_to_stream(out, first);
  __print(out, sep, true, left...);
}

template <class... Args>
void print(const Args&... args) {
  std::ostringstream out;
  __print(out, ", ", false, args...);
  out << "\n";
  std::cout << out.str();
}

template <class... Args>
void println(const Args&... args) {
  std::ostringstream out;
  __print(out, " ", false, args...);
  out << "\n";
  std::cout << out.str();
}

/**
 * Bufcout
 */
class Bufcout {
public:
  Bufcout() {}
  std::ostringstream out_;
  constexpr static Bufcout* endl = nullptr;
};

template <class T>
Bufcout&& operator<<(Bufcout&& out, T v) {
  out.out_ << std::boolalpha << v;
  return std::move(out);
}

inline Bufcout&& operator<<(Bufcout&& out, Bufcout* bc) {
  if (bc != Bufcout::endl) {
    return std::move(out);
  }
  out.out_ << "\n";
  std::cout << out.out_.str();
  return std::move(out);
}
