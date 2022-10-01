#include <header>

/*
119.cc

给定一个非负索引 row, 返回「杨辉三角」的第 row 行
row 从0开始
*/

vector<int> getRow(int row) {
  if (row < 0) return vector<int>{};
  if (row == 0) return vector<int>{1};
  if (row == 1) return vector<int>{1, 1};
  vector<int> curr(row+1, 1), next(row+1, 1);
  for (int ri = 2; ri <= row; ri++) {
    for (int i = 1; i < ri; i++) {
      next[i] = curr[i-1] + curr[i];
    }
    curr.swap(next);
  }
  return curr;
}

int main() {
  vector<int> ans{1,3,3,1};
  assert_eq(getRow(3), ans);
}


/*
120.cc

给定一个三角形 triangle ，找出自顶向下的最小路径和。

每一步只能移动到下一行中相邻的结点上。
相邻结点指下标与上一层结点下标相同或者等于上一层结点下标+1的两个结点。
即，当前行的下标i 下一步可以移动到下一行的下标 i 或 i + 1

例子：
输入：triangle = [[2],[3,4],[6,5,7],[4,1,8,3]]
输出：11
解释：如下面简图所示：
   2
  3 4
 6 5 7
4 1 8 3
自顶向下的最小路径和为 11（即，2 + 3 + 5 + 1 = 11）
*/

int minimumTotal(vector<vector<int>>& triangle) {
  const int n = (int)triangle.size();
  if (n == 0) return 0;
  vector<int> shortest(n, 0);
  // 第0层
  shortest[0] = triangle[0][0];
  for (int i = 1; i < n; i++) {
    // 第i层
    int prev = shortest[0];
    shortest[0] = triangle[i][0] + shortest[0];
    for (int j = 1; j < i; j++) {
      int curr = shortest[j];
      shortest[j] = triangle[i][j] + std::min(prev, curr);
      prev = curr;
    }
    shortest[i] = triangle[i][i] + prev;
  }
  //
  int ret = shortest[0];
  for (int i = 1; i < n; i++) {
    if (shortest[i] < ret) ret = shortest[i];
  }
  return ret;
}
