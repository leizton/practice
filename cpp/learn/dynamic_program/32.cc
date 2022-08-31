#include <util/base.h>

/*
给你一个只包含 '(' 和 ')' 的字符串，找出最长有效（格式正确且连续）括号子串的长度。

输入：s = ")()())"
输出：4
解释：最长有效括号子串是 "()()"
*/

int longestValidParentheses0(string s) {
  static int vmap[64];
  vmap['('] = 1;
  vmap[')'] = -1;
  int ans = -1;
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == ')') continue;
    int acc = vmap[s[i]];
    // 以i开始, 以j结尾
    for (int j = i+1; j < s.length(); j++) {
      acc += vmap[s[j]];
      if (acc == 0) {
        ans = std::max(ans, j-i);
      } else if (acc < 0) {
        break;
      }
    }
  }
  return ans+1;
}

/*
  考虑以i结尾的最长匹配串 ans(i) = s[j..i]

  if s[i] == '('
    ans(i) == 空串
  elif s[i] == ')'
    if ans(i-1) != 空串
      j = ans(i-1).begin - 1
      if s[j] == '('
        ans(i) = ans(j-1)是空串 ? s[j..i] : s[ans(j-1).begin..i]
    else
      if s[i-1] == '('
        ans(i) = ans(i-2)是空串 ? s[i-1..i] : s[ans(i-2).begin..i]
*/

int longestValidParentheses(string s) {
  const int N = s.length();
  // dp[i] 以i结尾的最长匹配串的长度, `i-dp[i]+1`是起始位置
  vector<int> dp(N, 0);
  int ans = 0;
  // 找到第一对括号
  int i = 1;
  for (; i < N; i++) {
    if (s[i-1] == '(' && s[i] == ')') {
      ans = dp[i] = 2;
      break;
    }
  }
  //
  for (i++; i < N; i++) {
    if (s[i] == ')') {
      bool flag = false;
      if (dp[i-1] == 0) {
        if (s[i-1] == '(') {
          dp[i] = 2;
          flag = true;
        }
      } else {
        int j = (i-1) - dp[i-1] + 1;
        if (j > 0 && s[j-1] == '(') {
          dp[i] = 2 + dp[i-1];
          flag = true;
        }
      }
      if (flag) {
        // 尝试扩展
        int j = i - dp[i] + 1;
        if (j > 0 && dp[j-1] > 0) {
          dp[i] += dp[j-1];
        }
        ans = std::max(ans, dp[i]);
      }
    }
  }
  return ans;
}
