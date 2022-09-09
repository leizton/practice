#include <header>

/*
130.cc
给你一个 m x n 的矩阵 grid ，由若干字符 'X' 和 'O'
找到所有被 'X' 围绕的区域，并将这些区域里所有的 'O' 用 'X' 填充
*/

void dfs(vector<vector<char>>& grid, int i, int j) {
  grid[i][j] = 'o';  // 标记成 live
  if (i > 0 && grid[i-1][j] == 'O') dfs(grid, i-1, j);
  if (i < grid.size()-1 && grid[i+1][j] == 'O') dfs(grid, i+1, j);
  if (j > 0 && grid[i][j-1] == 'O') dfs(grid, i, j-1);
  if (j < grid[0].size()-1 && grid[i][j+1] == 'O') dfs(grid, i, j+1);
}
void solve(vector<vector<char>>& grid) {
  if (grid.empty() || grid[0].empty()) return;
  int m = grid.size(), n = grid[0].size();
  // 只有边缘的'O'不被包围
  for (int j = 0; j < n; j++) {
    if (grid[0][j] == 'O') dfs(grid, 0, j);
    if (grid[m-1][j] == 'O') dfs(grid, m-1, j);
  }
  for (int i = 0; i < m; i++) {
    if (grid[i][0] == 'O') dfs(grid, i, 0);
    if (grid[i][n-1] == 'O') dfs(grid, i, n-1);
  }
  //
  for (int i = 1; i < m-1; i++) {
    for (int j = 1; j < n-1; j++) {
      if (grid[i][j] == 'O') grid[i][j] = 'X';
    }
  }
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (grid[i][j] == 'o') grid[i][j] = 'O';
    }
  }
}


/*
200.cc
给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量
边缘之外视作水
*/
void dfs(vector<vector<char>>& grid, int i, int j) {
  grid[i][j] = '0';
  if (i > 0 && grid[i-1][j] == '1') dfs(grid, i-1, j);
  if (i < grid.size()-1 && grid[i+1][j] == '1') dfs(grid, i+1, j);
  if (j > 0 && grid[i][j-1] == '1') dfs(grid, i, j-1);
  if (j < grid[0].size()-1 && grid[i][j+1] == '1') dfs(grid, i, j+1);
}
int numIslands(vector<vector<char>>& grid) {
  if (grid.empty() || grid[0].empty()) return 0;
  int m = grid.size(), n = grid[0].size();
  int ans = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (grid[i][j] == '1') {
        dfs(grid, i, j);
        ans++;
      }
    }
  }
  return ans;
}
