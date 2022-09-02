#include <util/base.h>

/*
给定一个仅包含 0 和 1 、大小为 rows x cols 的二维二进制矩阵，找出只包含 1 的最大矩形，并返回其面积。
*/

/*
  g(i,j)   第i行第j列的格子
  dp(i,j)  以g(i,j)为右下角的矩形中面积最大的矩形
           dp(i,j).h、dp(i,j).w、dp(i,j).s 分别是矩形的高、宽、面积
  H(i,j)   g(i,j)往上有多少个‘1’. if g(i,j)=‘0’, then h(i,j)=0
*/

/*
  dp(i,j) = max{
    H(i,j) × 1,
    min{ H(i,j-1) H(i,j) } × 2,
    min{ H(i,j-2) H(i,j-1) H(i,j) } × 3,
    ...
  }

  平均=O(m·n·n)
*/
int maximalRectangle0(vector<vector<char>>& mat) {
  if (mat.empty() || mat[0].empty()) return 0;
  int m = mat.size(), n = mat[0].size();
  //
  vector<vector<int>> H(m, vector<int>(n, 0));
  for (int j = 0; j < n; j++) {
    H[0][j] = (mat[0][j] == '0') ? 0 : 1;
  }
  for (int i = 1; i < m; i++) {
    for (int j = 0; j < n; j++) {
      H[i][j] = (mat[i][j] == '0') ? 0 : (1 + H[i-1][j]);
    }
  }
  //
  int ans = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      int h_min = H[i][j];
      int s_max = h_min;
      for (int k = j-1; k >= 0; k--) {
        h_min = std::min(h_min, H[i][k]);
        if (h_min <= 0) break;
        s_max = std::max(s_max, h_min * (j-k+1));
      }
      ans = std::max(ans, s_max);
    }
  }
  return ans;
}

/*
  单行最优值利用了84.cc的实现
  平均=O(m·n), 最坏=O(m·n·n)
*/
int largestRectangleArea(vector<int>& h, vector<pair<int,int>>& sk) {
  if (h.empty()) return 0;
  sk.clear();
  sk.emplace_back(0, 0);
  int ans = 0;
  for (int i = 1; i < h.size(); i++) {
    int left_bound_i = i;
    // try pop
    int right_bound = sk.back().first;
    while (!sk.empty()) {
      int pos = sk.back().first, left_bound = sk.back().second;
      if (h[i] <= h[pos]) {
        left_bound_i = left_bound;  // 继承左边界
        ans = std::max(ans, (right_bound - left_bound + 1) * h[pos]);
        sk.pop_back();
      } else {
        break;
      }
    }
    //
    sk.emplace_back(i, left_bound_i);
  }
  int right_bound = sk.back().first;
  while (!sk.empty()) {
    int pos = sk.back().first, left_bound = sk.back().second;
    ans = std::max(ans, (right_bound - left_bound + 1) * h[pos]);
    sk.pop_back();
  }
  return ans;
}
int maximalRectangle(vector<vector<char>>& mat) {
  if (mat.empty() || mat[0].empty()) return 0;
  int m = mat.size(), n = mat[0].size();
  //
  vector<vector<int>> H(m, vector<int>(n, 0));
  for (int j = 0; j < n; j++) {
    H[0][j] = (mat[0][j] == '0') ? 0 : 1;
  }
  for (int i = 1; i < m; i++) {
    for (int j = 0; j < n; j++) {
      H[i][j] = (mat[i][j] == '0') ? 0 : (1 + H[i-1][j]);
    }
  }
  //
  int ans = 0;
  vector<pair<int,int>> sk(n, {0, 0});
  for (int i = 0; i < m; i++) {
    ans = std::max(ans, largestRectangleArea(H[i], sk));
  }
  return ans;
}
