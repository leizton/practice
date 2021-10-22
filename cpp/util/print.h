#pragma once

#include <bits/stdc++.h>

template <class K, class V>
std::ostream& operator<<(std::ostream& out, std::pair<K, V> p) {
  out << "(" << p.first << "," << p.second << ")";
  return out;
}

template <class Container>
std::ostream& print_seq_container(std::ostream& out, const Container& c) {
  out << "[";
  bool first = true;
  for (const auto& e : c) {
    if (!first) out << ",";
    first = false;
    out << e;
  }
  out << "]";
  return out;
}

template <class Container>
std::ostream& print_map_container(std::ostream& out, const Container& c) {
  out << "[";
  bool first = true;
  for (const auto& p : c) {
    if (!first) out << ",";
    first = false;
    out << p.first << "=" << p.second;
  }
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

template <class T>
std::string to_str(const T& v) {
  std::ostringstream ss;
  ss << v;
  return ss.str();
}

/**
 * void print(Args... args)
 */
inline void __print(std::ostringstream& out, std::string sep, bool is_begin) {
}

template <class First, class... Args>
void __print(std::ostringstream& out, std::string sep, bool is_begin, const First& first, const Args&... left) {
  if (!is_begin) out << sep;
  out << first;
  __print(out, sep, false, left...);
}

template <class... Args>
void print(const Args&... args) {
  std::ostringstream out;
  out << std::boolalpha;
  __print(out, ", ", true, args...);
  out << "\n";
  std::cout << out.str();
}

template <class... Args>
void println(const Args&... args) {
  std::ostringstream out;
  out << std::boolalpha;
  __print(out, " ", true, args...);
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
