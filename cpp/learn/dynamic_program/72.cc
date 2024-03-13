#include <util/base.h>

/*
最小编辑距离
给你两个单词 w1 和 w2，请返回将 w1 转换成 w2 所使用的最少操作数
你可以对一个单词进行如下三种操作：
  - 加一个字符
  - 减一个字符
  - 改一个字符
*/

int minDistance(string s, string t) {
  int m = (int)s.size(), n = (int)t.size();
  if (m == 0 || n == 0) return max(m, n);

  vector<vector<int>> dp(m, vector<int>(n, 0));
  dp[0][0] = (s[0] == t[0]) ? 0 : 1;

  for (int i = 1; i < m; i++) {
    dp[i][0] = (s[i] == t[0]) ? i : (dp[i-1][0] + 1);
  }
  for (int j = 1; j < n; j++) {
    dp[0][j] = (s[0] == t[j]) ? j : (dp[0][j-1] + 1);
  }

  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      dp[i][j] = (s[i] == t[j]) ? dp[i-1][j-1] : (min(dp[i-1][j], min(dp[i][j-1], dp[i-1][j-1])) + 1);
    }
  }
  return dp[m-1][n-1];
}
