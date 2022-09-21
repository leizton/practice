#include <util/base.h>

int main() {
  int sz = 97;
  std::vector<int> a;
  for (int i = 0; i < sz; i++) a.push_back(i);

  std::vector<int> check_bitset;
  check_bitset.resize(sz);

  for (int i = 0; i < sz; i++) {
    std::random_shuffle(a.begin(), a.end());
    select_topk(i, a.begin(), a.end(), std::less<int>());

    int n = 0;
    for (int j = 0; j < i; j++) check_bitset[j] = 0;
    for (int j = 0; j < i; j++) check_bitset[a[j]] = 1;
    for (int j = 0; j < i; j++) n += check_bitset[j];
    if (n != i) println("error", n, i);
  }
}