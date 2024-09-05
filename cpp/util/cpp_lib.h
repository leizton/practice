#pragma once

// #include <bits/stdc++.h>

#include <cstdint>
#include <limits>
#include <new>
#include <numeric>
#include <typeinfo>
#include <utility>
#include <cmath>
#include <chrono>
#include <initializer_list>
#include <ratio>
#include <scoped_allocator>
#include <typeindex>
#include <type_traits>
#include <exception>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iterator>
#include <locale>
#include <stdexcept>
#include <typeinfo>
#include <utility>
#include <codecvt>
#include <random>
#include <regex>
#include <system_error>

#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>
#include <sstream>
#include <streambuf>

#include <iterator>
#include <tuple>
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <forward_list>
#include <valarray>

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>

#include <algorithm>
#include <functional>
#include <memory>
#include <initializer_list>

#if __cplusplus >= 201703L
#include <any>
#include <optional>
#include <variant>
#include <string_view>
#endif

#if __cplusplus >= 202002L
#include <bit>
#include <compare>
#include <concepts>
#include <numbers>
#include <ranges>
#include <span>
#include <source_location>
#include <version>
#include <expected>
#include <stdatomic.h>
#endif

using namespace std;

#define reload_os_out(type) friend std::ostream& operator<<(std::ostream& out, const type& obj)

inline uint64_t ptrToInt(const void* p) {
  return reinterpret_cast<uint64_t>(p);
}
