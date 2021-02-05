#include "util/base.h"

def(find_rfind) {
  string s = "a/b/c/d";
  assert_eq(std::string::npos, s.find('\\'));
  assert_eq(1u, s.find('/'));
  assert_eq(3u, s.find('/', 2));
  assert_eq(5u, s.rfind('/'));
}

def(starts_with_ends_with) {
  auto starts_with = [](const string& s, const string& prefix) {
    if (s.length() < prefix.length()) return false;
    return strncmp(s.c_str(), prefix.c_str(), prefix.length()) == 0;
  };

  auto ends_with = [](const string& s, const string& prefix) {
    const size_t n = prefix.length();
    if (s.length() < n) return false;
    return strncmp(s.c_str() + (s.length() - n), prefix.c_str(), n) == 0;
  };

  string s = "12345";
  assert_T(starts_with(s, "123"));
  assert_T(starts_with(s, ""));
  assert_F(starts_with(s, "abc"));
  assert_F(starts_with(s, "123456"));

  assert_T(ends_with(s, "345"));
  assert_T(ends_with(s, ""));
  assert_F(ends_with(s, "abc"));
  assert_F(ends_with(s, "012345"));
}

def(dirpath) {
  auto getDirpath = [](const string& path) -> string {
    auto idx = path.rfind('/');
    return (idx == std::string::npos) ? "" : path.substr(0, idx + 1);
  };
  assert_eq("/home/", getDirpath("/home/a.txt"));
  assert_eq("/home/lz/", getDirpath("/home/lz/a.txt"));
  assert_eq("", getDirpath("a.txt"));
}

def(string_deep_or_shallow_copy) {
  // 都是 deep-copy. 不同gcc结果不同
  char a[10] = "123";
  string s(a);
  assert_neq(ptrToInt(a), ptrToInt(s.c_str()));
  string s1(s);
  assert_neq(ptrToInt(s.c_str()), ptrToInt(s1.c_str()));

  const int N = 3;
  TrivialInt arr[N];
  for (int i = 0; i < N; i++)
    arr[i].v = 0;

  // arr 到 bs 是深拷贝
  basic_string<TrivialInt> bs(arr, arr + 3);
  assert_neq(ptrToInt(arr), ptrToInt(bs.c_str()));

  // bs 到 bs1 是深拷贝
  basic_string<TrivialInt> bs1(bs);
  assert_neq(ptrToInt(bs.c_str()), ptrToInt(bs1.c_str()));
}

int str2int64(const char* s, int64_t& res, int base = 0) {
  register int c;
  if (s == nullptr) return 1;
  if (!(base == 2 || base == 8 || base == 10 || base == 16)) base = 0;

  // skip white space
  while (isspace(*s))
    s++;

  // pick up +/- sign
  bool is_negative = false;
  c = *s++;
  if (c == '-') {
    is_negative = true;
    c = *s++;
  } else if (c == '+') {
    c = *s++;
  }

  // 0[b|B] => base=2
  // 0[x|X] => base=16
  if (c == '0') {
    c = *s++;
    if (c == 'b' || c == 'B') {
      if (base != 0 && base != 2) return 1;
      base = 2;
      c = *s++;
    } else if (c == 'x' || c == 'X') {
      if (base != 0 && base != 16) return 1;
      base = 16;
      c = *s++;
    } else if (base == 0) {
      base = 8;
    }
  } else {
    base = 10;
  }

  register uint64_t val_limit = is_negative ? INT64_MIN : INT64_MAX;
  register uint64_t checkpoint = val_limit / base;

  register uint64_t val = 0;
  while (true) {
    if (isdigit(c))
      c -= '0';
    else if (isalpha(c))
      c -= (c <= 'Z') ? ('A' - 10) : ('a' - 10);
    else
      break;
    if (c >= base)
      return 1;

    if (val >= checkpoint) {
      if (val * base + c > val_limit)
        return 2; // out of range
    }
    val *= base;
    val += c;
    c = *s++;
  }

  res = (int64_t)val;
  if (is_negative) res = -res;
  return 0;
}

run(strtol) {
  int64_t val = 0;

#define CHECK(expect, s)           \
  assert_eq(0, str2int64(s, val)); \
  assert_eq(expect, val);

  CHECK(123, " 123 ");
  CHECK(123, " 0b01111011");
  CHECK(123, " 0173");
  CHECK(123, " 00173");
  CHECK(123, " 0x07B");

  CHECK(+123, "+123");
  CHECK(-123, "-123");

  CHECK(INT64_MAX, " +9223372036854775807");
  CHECK(INT64_MIN, " -9223372036854775808");
#undef CHECK

  assert_eq(2, str2int64("9223372036854775808", val));
  assert_eq(2, str2int64("-9223372036854775809", val));

  assert_eq(0, str2int64(" 00123", val, 10));
  assert_eq(123, val);
}

main_run;