#include "util/algo.h"

bool str2int(const std::string& s, int& v) {
  if (s.empty()) return false;
  try {
    size_t p = 0u;
    int t = std::stoi(s.c_str(), &p);
    if (p == s.length()) {
      v = t;
      return true;
    } else {
      return false;
    }
  } catch (std::exception& ex) {
    return false;
  }
}

bool str2int64(const std::string& s, int64_t& v) {
  if (s.empty()) return false;
  char* p = nullptr;
  int64_t t = std::strtoll(s.c_str(), &p, 10);
  if (p && *p == '\0') {
    v = t;
    return true;
  } else {
    return false;
  }
}

bool str2uint64(const std::string& s, uint64_t& v) {
  if (s.empty()) return false;
  char* p = nullptr;
  uint64_t t = std::strtoull(s.c_str(), &p, 10);
  if (p && *p == '\0') {
    v = t;
    return true;
  } else {
    return false;
  }
}

bool str2float(const std::string& s, float& v) {
  if (s.empty()) return false;
  char* p = nullptr;
  float t = std::strtof(s.c_str(), &p);
  if (p && *p == '\0') {
    v = t;
    return true;
  } else {
    return false;
  }
}

bool str2double(const std::string& s, double& v) {
  if (s.empty()) return false;
  char* p = nullptr;
  double t = std::strtod(s.c_str(), &p);
  if (p && *p == '\0') {
    v = t;
    return true;
  } else {
    return false;
  }
}

std::string double2Str(double d, int precision) {
  if (d == int64_t(d)) {
    return std::to_string(int64_t(d));
  } else {
    auto s = std::to_string(d - int64_t(d));
    return std::to_string(int64_t(d)) + s.substr(1, std::min(precision, (int)s.find_last_not_of('0')));
  }
}

bool startswith(const std::string& s, const std::string& prefix) {
  size_t n = prefix.length();
  if (n <= s.length()) {
    return ::strncmp(s.c_str(), prefix.c_str(), n) == 0;
  } else {
    return false;
  }
}

void splitString(const std::string& str, const std::string& delimiter,
                 std::vector<std::string>& out) {
  out.clear();
  size_t i = 0;
  while (i <= str.length()) {
    size_t j = str.find(delimiter, i);
    if (j == std::string::npos) {
      out.push_back(str.substr(i, str.length() - i));
      break;
    }
    out.push_back(str.substr(i, j - i));
    i = j + delimiter.length();
  }
}
