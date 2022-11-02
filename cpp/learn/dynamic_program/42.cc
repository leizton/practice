#include <util/base.h>

/*
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。
*/

/**
 * 当前位置能存多少取决于左右两边的max
*/
int trap(vector<int>& h) {
  if (h.size() <= 2) return 0;
  const int n = h.size();

  // left_dp[i] = max{ h[0], h[1], ..., h[i] }
  vector<int> left_dp(n, 0);
  left_dp[0] = h[0];
  for (int i = 1; i < n; i++) {
    left_dp[i] = std::max(left_dp[i-1], h[i]);
  }

  // right_dp[i] = max{ h[i], h[i+1], ..., h[n-1] }
  vector<int> right_dp(n, 0);
  right_dp[n-1] = h[n-1];
  for (int i = n-2; i >= 0; i--) {
    right_dp[i] = std::max(right_dp[i+1], h[i]);
  }

  int res = 0;
  for (int i = 1; i < n-1; i++) {
    res += std::min(left_dp[i], right_dp[i]) - h[i];
  }
  return res;
}

int main() {
  auto test = [](vector<int> h) {
    return trap(h);
  };
  assert_eq(6, test({0,1,0,2,1,0,1,3,2,1,2,1}));
  assert_eq(13, test({5,2,0,3,2,5}));
}
