#include <header>

/*
  62.cc附加
  grid[i][j] == 0 无障碍
  grid[i][j] == 1 有障碍，不能经过该网格
  问有多少种路径
*/

int uniquePathsWithObstacles(vector<vector<int>>& grid) {
  if (grid.empty() || grid[0].empty() || grid[0][0] == 1) return 0;
  int m = grid.size(), n = grid[0].size();

  grid[0][0] = 1;
  for (int i = 1; i < m; i++) {
    grid[i][0] = grid[i-1][0] * (1 - grid[i][0]);
  }
  for (int j = 1; j < n; j++) {
    grid[0][j] = grid[0][j-1] * (1 - grid[0][j]);
  }

  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      grid[i][j] = (1 - grid[i][j]) * (grid[i-1][j] + grid[i][j-1]);
    }
  }
  return grid[m-1][n-1];
}
