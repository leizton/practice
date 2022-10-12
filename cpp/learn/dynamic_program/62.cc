#include <util/base.h>

/*
一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为 “Start” ）。
机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为 “Finish” ）。
问总共有多少条不同的路径？
*/

int uniquePaths(int m, int n) {
  if (m <= 1 || n <= 1) return 1;
  if (n < m) std::swap(m, n);
  vector<int> dp(n, 1);
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      dp[j] += dp[j-1];
    }
  }
  return dp[n-1];
}

int main() {
  assert_eq(5, uniquePaths(5, 2));
  assert_eq(5, uniquePaths(2, 5));
  assert_eq(28, uniquePaths(7, 3));
  assert_eq(70, uniquePaths(5, 5));
  assert_eq(1916797311, uniquePaths(51, 9));
}
