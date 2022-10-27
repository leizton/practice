#include <header>

/*
给定m×n网格, 网格内字符是 0 or 1
找出全是1构成的最大正方形
*/

int maximalSquare(vector<vector<char>>& mat) {
  if (mat.empty() || mat[0].empty()) return 0;
  int m = mat.size(), n = mat[0].size();

  // dp[i,j] 表示以[i,j]格子作右下角的最大正方形的边长
  // if mat[i,j]=0 then dp[i,j]=0
  vector<vector<int>> dp(m, vector<int>(n, 0));

  // 垂直方向连续'1'个数
  vector<int> vertical_one_num(n, 0);

  int max_edge = 0;
  for (int j = 0; j < n; j++) {
    dp[0][j] = vertical_one_num[j] = mat[0][j] - '0';
    max_edge |= dp[0][j];
  }
  for (int i = 1; i < m; i++) {
    dp[i][0] = mat[i][0] - '0';
    max_edge |= dp[i][0];
  }

  for (int i = 1; i < m; i++) {
    int horizontal_one_num = mat[i][0] - '0';
    for (int j = 1; j < n; j++) {
      if (mat[i][j] == '0') {
        horizontal_one_num = 0;
        vertical_one_num[j] = 0;
        dp[i][j] = 0;
        continue;
      }
      dp[i][j] = 1 + std::min({dp[i-1][j-1], horizontal_one_num, vertical_one_num[j]});
      max_edge = std::max(max_edge, dp[i][j]);
      horizontal_one_num += 1;
      vertical_one_num[j] += 1;
    }
  }
  return max_edge * max_edge;
}

int maximalSquare_opt(vector<vector<char>>& mat) {
  if (mat.empty() || mat[0].empty()) return 0;
  int m = mat.size(), n = mat[0].size();

  vector<int> dp_prev(n, 0);
  vector<int> dp_curr(n, 0);

  int max_edge = 0;
  for (int j = 0; j < n; j++) {
    dp_prev[j] = mat[0][j] - '0';
    max_edge |= dp_prev[j];
  }
  for (int i = 1; i < m; i++) {
    max_edge |= mat[i][0] - '0';
  }

  for (int i = 1; i < m; i++) {
    dp_curr[0] = mat[i][0] - '0';
    for (int j = 1; j < n; j++) {
      if (mat[i][j] == '0') {
        dp_curr[j] = 0;
      } else {
        dp_curr[j] = 1 + std::min({dp_curr[j-1], dp_prev[j-1], dp_prev[j]});
        max_edge = std::max(max_edge, dp_curr[j]);
      }
    }
    dp_prev.swap(dp_curr);
  }
  return max_edge * max_edge;
}
