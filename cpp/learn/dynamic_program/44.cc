#include <util/base.h>

/*
给定一个字符串 (s) 和一个字符模式 (p) ，实现一个支持 '?' 和 '*' 的通配符匹配。
两个字符串完全匹配才算匹配成功。
*/


/*
  match(p[0..m-1], s[0..n-1])的递归式
  if p[0] == *
    match(p[1..], s[0..])
    or match(p[1..], s[1..])
    or ...
    or match(p[1..], s[n-1])
    or match(p[1..], "")
  elif p[0] == ? or p[0] == s[0]
    match(p[1..], s[1..])
  else
    false

  ** 把p[0]换成p[i], 从后向前递归 **

  match(p[0..i], s[0..j])的递归如下
  if p[i] == *
    match(p[0..i-1], s[0..j])
    or match(p[0..i-1], s[0..j-1])  ;p[i]匹配s[j]
    or match(p[0..i-1], s[0..j-2])  ;p[i]匹配s[j-1..j]
    or ...
    or match(p[0..i-1], s[0])
    or match(p[0..i-1], "")
  elif p[i] == ? or p[i] == s[j]
    match(p[0..i-1], s[0..j-1])
  else
    false
*/

// DP on Strings
bool isMatch(string s, string p) {
  const int M = p.length(), N = s.length();
  vector<vector<bool>> dp(M+1, vector<bool>(N+1, false));
  dp[0][0] = true;
  // p是空串
  for (int j = 1; j <= N; j++) {
    dp[0][j] = false;
  }
  // s是空串
  for (int i = 1; i <= M; i++) {
    dp[i][0] = (p[i-1] == '*') && dp[i-1][0];
  }
  //
  for (int i = 1; i <= M; i++) {
    if (p[i-1] == '*') {
      for (int j = 0; j <= N; j++) {
        if (dp[i-1][j]) {
          for (; j <= N; j++) dp[i][j] = true;
          break;
        }
      }
    } else if (p[i-1] == '?') {
      for (int j = 1; j <= N; j++) {
        dp[i][j] = dp[i-1][j-1];
      }
    } else {
      for (int j = 1; j <= N; j++) {
        dp[i][j] = (p[i-1] == s[j-1]) && dp[i-1][j-1];
      }
    }
  }
  return dp[M][N];
}

int main() {
  std::function<bool(string,string)> test = isMatch;
  assert_F(test("aa", "a"));
  assert_T(test("aa", "*"));
  assert_T(test("ab", "?*"));
  assert_F(test("cb", "?a"));
  assert_T(test("adceb", "*a*b"));
  assert_F(test("acdcb", "a*c?b"));
  assert_F(test("mississippi", "m??*ss*?i*pi"));
}
