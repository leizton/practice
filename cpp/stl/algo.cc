#include "util/base.h"


def(lower_upper_bound) {
  vector<int> a{1,2,2,4,5};
  auto lower = [&a] (int target) {
    return std::lower_bound(a.begin(), a.end(), target) - a.begin();
  };
  auto upper = [&a] (int target) {
    return std::upper_bound(a.begin(), a.end(), target) - a.begin();
  };

  // lower_bound() 返回target比多少个元素大(不包括等于)
  assert_eq(0, lower(-1));
  assert_eq(1, lower(2));
  assert_eq(3, lower(3));
  assert_eq(5, lower(6));

  // upper_bound() 返回target比多少个元素大(包括等于)
  assert_eq(0, upper(-1));
  assert_eq(3, upper(2));
  assert_eq(3, upper(3));
  assert_eq(5, upper(6));
}


#if comment(sort)
sort<RandomIter>(RandomIter begin, RandomIter end)  // operator<
sort<RandomIter, Compare>(RandomIter begin, RandomIter end, Compare cmp)
#endif
def(sort) {
  vector<Int> v{2, 1, 3};
  std::sort(v.begin(), v.end());
  assert_eq("[1,2,3]", to_str(v));

  std::sort(v.begin(), v.end(), [](const Int& x, const Int& y){
    return y.v < x.v;
  });
  assert_eq("[3,2,1]", to_str(v));
}


main_run;