#include "util/base.h"

#if comment(sort)
sort<RandomIter>(RandomIter begin, RandomIter end); // operator<
sort<RandomIter, Compare>(RandomIter begin, RandomIter end, Compare cmp);
#endif

run(sort) {
  vector<Int> v{2, 1, 3};
  std::sort(v.begin(), v.end());
  assert_eq("[1,2,3]", to_str(v));

  std::sort(v.begin(), v.end(), std::greater<Int>()); // need Int overload operator>
  assert_eq("[3,2,1]", to_str(v));
}

main_run;
