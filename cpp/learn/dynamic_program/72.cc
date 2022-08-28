#include <util/base.h>

/*
最小编辑距离
给你两个单词 word1 和 word2，请返回将 word1 转换成 word2 所使用的最少操作数
你可以对一个单词进行如下三种操作：
- 加一个字符
- 减一个字符
- 改一个字符
*/

/*
  ans(s[0..i], t[0..j])的递归式
  if s[i] == t[j]
    ;只需把 s[0..i-1] 变成 t[0..j-1]
    ans(s[0..i-1], t[0..j-1])
  else
    ;对于不相等的处理有3种方法
    min(
      ans(s[0..i], t[0..j-1])   + 1  ; s 加 t[j]
      ans(s[0..i-1], t[0..j])   + 1  ; s 丢弃 s[i]
      ans(s[0..i-1], t[0..j-1]) + 1  ; s[i]->t[j]
    )
*/

int minDistance(string s, string t) {
  const int M = s.length(), N = t.length();
  vector<vector<int>> dp(M+1, vector<int>(N+1, 0));
  // s是空串
  for (int j = 1; j <= N; j++) {
    dp[0][j] = j;
  }
  // t是空串
  for (int i = 1; i <= M; i++) {
    dp[i][0] = i;
  }
  //
  for (int i = 1; i <= M; i++) {
    for (int j = 1; j <= N; j++) {
      if (s[i-1] == t[j-1]) {
        dp[i][j] = dp[i-1][j-1];
      } else {
        dp[i][j] = std::min(dp[i-1][j], std::min(dp[i][j-1], dp[i-1][j-1])) + 1;
      }
    }
  }
  return dp[M][N];
}
