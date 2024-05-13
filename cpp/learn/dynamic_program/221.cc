#include <header>

/*
给定m×n网格, 网格内字符是 0 or 1
找出全是1构成的最大正方形
*/

int maximalSquare(vector<vector<char>>& mat) {
  if (mat.empty() || mat[0].empty()) return 0;
  int m = (int)mat.size(), n = (int)mat[0].size();
  int max_side = 0;
  vector<int> dp(n, 0), prev_dp(n, 0), horizon_dp(n, 0), vertical_dp(n, 0);
  for (int j = 0; j < n; j++) {
    if (mat[0][j] == '1') max_side = dp[j] = vertical_dp[j] = 1;
  }
  for (int i = 1; i < m; i++) {
    prev_dp = dp;
    horizon_dp.assign(n, 0);
    if (mat[i][0] == '1') {
      dp[0] = horizon_dp[0] = 1;
      vertical_dp[0] += 1;
      max_side = std::max(max_side, 1);
    } else {
      dp[0] = horizon_dp[0] = vertical_dp[0] = 0;
    }
    for (int j = 1; j < n; j++) {
      if (mat[i][j] == '1') {
        dp[j] = 1 + std::min(prev_dp[j-1], std::min(horizon_dp[j-1], vertical_dp[j]));
        horizon_dp[j] = horizon_dp[j-1] + 1;
        vertical_dp[j] += 1;
        max_side = std::max(max_side, dp[j]);
      } else {
        dp[j] = horizon_dp[j] = vertical_dp[j] = 0;
      }
    }
  }
  return max_side * max_side;
}