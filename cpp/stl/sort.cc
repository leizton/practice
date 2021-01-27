#include "util/base.h"

#if comment(sort)
sort<RandomIter>(RandomIter begin, RandomIter end); // operator<
sort<RandomIter, Compare>(RandomIter begin, RandomIter end, Compare cmp);
#endif

def(sort) {
  vector<Int> v{2, 1, 3};
  std::sort(v.begin(), v.end());
  assert_eq("[1,2,3]", to_str(v));

  std::sort(v.begin(), v.end(), std::greater<Int>()); // need Int overload operator>
  assert_eq("[3,2,1]", to_str(v));
}

std::atomic<int> ObjId(1);

struct Obj {
  int v;
  int id = ObjId++;
  Obj(int _v) { v = _v; }
  bool operator<(const Obj& o) const { return v < o.v; }
};

ostream& operator<<(ostream& os, const Obj& o) {
  os << o.id;
  return os;
}

#define ENABLE_STABLE_QSORT 1

template <class T>
int qsortPart(vector<T>& vec, int begin, int end) {
  const auto& ref = vec[begin];
  int i = begin + 1, j = end - 1;
  while (i < j) {
    if (vec[i] < ref) {
      i++;
    } else {
#if ENABLE_STABLE_QSORT
      while (i < j) {
        if (vec[j] < ref) {
          std::swap(vec[i], vec[j]);
          break;
        }
        j--;
      }
#else
      std::swap(vec[i], vec[j]);
      j--;
#endif
    }
  }
  if (vec[i] < ref) {
    std::swap(vec[i], vec[begin]);
  } else {
    i--;
    if (i > begin) {
      std::swap(vec[i], vec[begin]);
    }
  }
  return i;
}

template <class T>
void qsort(vector<T>& vec, int begin, int end) {
  if (end - begin <= 1) {
    return;
  }
  if (end - begin == 2) {
    if (vec[begin + 1] < vec[begin]) {
      std::swap(vec[begin], vec[begin + 1]);
      return;
    }
  }
  int p = qsortPart(vec, begin, end);
  qsort(vec, begin, p);
  qsort(vec, p + 1, end);
}

run(sort stability) {
  vector<Obj> v;
  auto gen = []() -> vector<Obj> {
    ObjId = 1;
    vector<Obj> v;
    v.reserve(20);
    for (int i = 0; i < 20; i++) {
      v.emplace_back(1);
    }
    return v;
  };

  v = gen();
  std::sort(v.begin(), v.end());
  println("std::sort", v);

  v = gen();
  std::stable_sort(v.begin(), v.end());
  println("std::stable_sort", v);

  v = gen();
  qsort(v, 0, (int)v.size());
  println("my::qsort", v);
}

main_run;
