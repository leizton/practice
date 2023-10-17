#pragma once

#include <bits/stdc++.h>

/*
 * sort
 */
template <typename RandomIter, typename Less>
void insert_sort(RandomIter begin, RandomIter end, Less less) {
  using Value = typename std::iterator_traits<RandomIter>::value_type;
  auto thre_end = begin + 7;
  if (thre_end > end) thre_end = end;
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

// end - begin >= 5
template <typename RandomIter, typename Less>
RandomIter q_partition(RandomIter begin, RandomIter end, Less less) {
  auto pivot = begin, p = begin+1, q = end-1;
  if (less(*p, *pivot)) {
    if (less(*pivot, *q)) { }
    else if (less(*p, *q)) { std::iter_swap(pivot, q); }
    else { std::iter_swap(pivot, q); std::iter_swap(p, pivot); }
  } else {
    if (less(*p, *q)) { std::iter_swap(p, pivot); }
    else if (less(*pivot, *q)) { std::iter_swap(p, pivot); std::iter_swap(pivot, q); }
    else { std::iter_swap(p, q); }
  }
  //
  pivot = begin, p = begin+2, q = end-2;
  while (true) {
    while (less(*p, *pivot)) p++;  // [end-1]是哨兵, begin+2<=p<=end-1
    while (less(*pivot, *q)) q--;  // [begin+1]是哨兵, begin+1<=q<=end-2
    if (p >= q) break;
    std::iter_swap(p, q);
    p++, q--;
  }
  // begin+2<=p<=end-1 && a[begin]=pivot && a[p-1]<pivot
  return p;
}

// std::nth_element(begin, begin+k, end, less);
template <typename RandomIter, typename Less>
void select_topk(int k, RandomIter begin, RandomIter end, Less less) {
  if (k <= 0 || (end-begin) <= k) return;
  auto target = begin + k;
  while (end - begin >= 5) {
    auto pivot = q_partition(begin, end, less);
    if (pivot < target)
      begin = pivot;
    else if (pivot == target)
      return;
    else
      end = pivot;
  }
  insert_sort(begin, end, less);
}

float wilsonIntervalRatio(int numerator, int denominator);
