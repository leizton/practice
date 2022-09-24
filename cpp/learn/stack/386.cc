#include <header>

/*
给你一个整数 n ，按字典序返回范围 [1, n] 内所有整数。
你必须设计一个时间复杂度为 O(n) 且使用 O(1) 额外空间的算法。

输入：n = 13
输出：[1,10,11,12,13,2,3,4,5,6,7,8,9]
*/

vector<int> lexicalOrder(int n) {
  if (n < 1) return vector<int>();
  vector<int> ret;
  ret.reserve(n);
  vector<pair<int, bool>> stk;
  stk.reserve(32);

  ret.emplace_back(1);
  stk.emplace_back(1, true);
  while (!stk.empty()) {
    if (stk.back().second) {
      // 尝试追加一个0
      stk.back().second = false;
      int j = stk.back().first * 10;
      if (j == n) {
        ret.emplace_back(j);
      } else if (j < n) {
        ret.emplace_back(j);
        stk.emplace_back(j, true);
      } else {
        int k = stk.back().first;
        stk.pop_back();
        int j = k + 1;
        k = std::min((k - (k%10) + 9), n);
        for (; j <= k; j++) ret.emplace_back(j);
      }
    } else {
      // stk.back() 已经尝试过追加0
      int k = stk.back().first;
      stk.pop_back();
      int j = k + 1;
      k = std::min((k - (k%10) + 9), n);
      if (j <= k) {
        ret.emplace_back(j);
        stk.emplace_back(j, true);
      }
    }
  }
  return ret;
}

vector<int> lexicalOrder_(int n) {
  vector<int> ret(n);
  int number = 1;
  for (int i = 0; i < n; i++) {
    ret[i] = number;
    if (number * 10 <= n) {
      // 成功追加一个0, 所以 next = number * 10
      number *= 10;
    } else {
      while (number%10 == 9 || n <= number+1) {
        number /= 10;
      }
      number++;
    }
  }
  return ret;
}

int main() {
  int n = 1;
  auto ret = lexicalOrder(n);
  assert_eq(n, (int)ret.size());
}
