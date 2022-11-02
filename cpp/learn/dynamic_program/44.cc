#include <util/base.h>

/*
给定一个字符串 (s) 和一个字符模式 (p) ，实现一个支持 '?' 和 '*' 的通配符匹配。
两个字符串完全匹配才算匹配成功。
*/

bool isMatch(string s, string p) {
  const int m = p.length(), n = s.length();

  // dp[i][j]: p[0,i)和s[0,j)是否匹配
  vector<vector<bool>> dp(m+1, vector<bool>(n+1, false));
  dp[0][0] = true;
  // p是空串
  for (int j = 1; j <= n; j++) {
    dp[0][j] = false;
  }
  // s是空串
  for (int i = 1; i <= m; i++) {
    dp[i][0] = (p[i-1] == '*') && dp[i-1][0];
  }

  for (int i = 1; i <= m; i++) {
    if (p[i-1] == '*') {
      for (int j = 0; j <= n; j++) {
        // 左边
        if (dp[i-1][j]) {
          for (; j <= n; j++) dp[i][j] = true;
          break;
        }
      }
    } else {
      // 斜上方
      for (int j = 1; j <= n; j++) {
        dp[i][j] = (p[i-1] == '?' || p[i-1] == s[j-1]) && dp[i-1][j-1];
      }
    }
  }
  return dp[m][n];
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
