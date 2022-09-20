#pragma once

#include "util/cpp_lib.h"

bool str2int(const std::string& s, int& v);
bool str2int64(const std::string& s, int64_t& v);
bool str2uint64(const std::string& s, uint64_t& v);
bool str2float(const std::string& s, float& v);
bool str2double(const std::string& s, double& v);

std::string double2Str(double d, int precision = 4);

bool startswith(const std::string& s, const std::string& prefix);

void splitString(const std::string& str, const std::string& delimiter,
                 std::vector<std::string>& out);

/**
 * sort
 */
template <typename RandomIter, typename Less>
void insert_sort(RandomIter begin, RandomIter end, Less less) {
  using Value = typename std::iterator_traits<RandomIter>::value_type;
  auto thre_end = begin + 7;
  auto p = begin;
  for (auto i = p+1; i < thre_end; ++i) {
    if (!less(*i, *p)) { ++p; continue; }
    Value v = std::move(*i);
    *i = std::move(*p);
    for (auto j = p-1; j >= begin && less(v, *j); --p, --j) *p = std::move(*j);
    *p = std::move(v);
    p = i;
  }
  for (auto i = thre_end; i < end; ++i) {
    if (!less(*i, *p)) { ++p; continue; }
    Value v = std::move(*i);
    p = std::upper_bound(begin, p, v);  // p ∈ [begin, i-1]
    for (auto j = i-1; j >= p; --j) *(j+1) = std::move(*j);
    *p = std::move(v);
    p = i;
  }
}
