#include <util/base.h>

/*
给定一个包含非负整数的 m x n 网格 grid ，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。
每次只能向下或者向右移动一步。
*/

int minPathSum(vector<vector<int>>& grid) {
  if (grid.empty() || grid[0].empty()) return 0;
  int m = grid.size(), n = grid[0].size();
  vector<int> dp(m);
  dp[0] = grid[0][0];
  for (int i = 1; i < m; i++) {
    dp[i] = dp[i-1] + grid[i][0];
  }
  for (int j = 1; j < n; j++) {
    dp[0] += grid[0][j];
    for (int i = 1; i < m; i++) {
      dp[i] = std::min(dp[i], dp[i-1]) + grid[i][j];
    }
  }
  return dp[m-1];
}
