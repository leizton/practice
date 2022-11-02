#include <util/base.h>

/*
最小编辑距离
给你两个单词 w1 和 w2，请返回将 w1 转换成 w2 所使用的最少操作数
你可以对一个单词进行如下三种操作：
  - 加一个字符
  - 减一个字符
  - 改一个字符
*/

int minDistance(string p, string s) {
  const int m = p.length(), n = s.length();

  // dp[i][j]: p[0:i-1]变成s[0:j-1]的最小编辑距离
  vector<vector<int>> dp(m+1, vector<int>(n+1, 0));
  // s是空串
  for (int j = 1; j <= n; j++) {
    dp[0][j] = j;
  }
  // t是空串
  for (int i = 1; i <= m; i++) {
    dp[i][0] = i;
  }

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      // 推导dp[i][j]
      dp[i][j] = std::min({
        dp[i-1][j-1] + (p[i-1]==s[j-1] ? 0 : 1),  // 修改字符. p[i-1]变成s[j-1]
        dp[i-1][j] + 1,  // 加入字符p[j-1]. p[0,i-2] s[0,j-1] => p[0,i-1] s[0,j-1]
        dp[i][j-1] + 1,  // 加入字符s[j-1]. p[0,i-1] s[0,j-2] => p[0,i-1] s[0,j-1]
      });
    }
  }
  return dp[m][n];
}
