#include <util/base.h>

/*
给你一个字符串 s，找到 s 中最长的回文子串
*/

/*
  首先回文属于子串, 所以按子串建模
  考虑 dp[i]=以i结尾的最长回文子串的长度
  推导dp[i]时 是从 s[i] s[i-1:i] s[i-2:i] ... s[0:i] 得出
  dp[i] = max{
    i+1 if s[i]==s[0] && is_palindrome(1,i-1)
    i   if s[i]==s[1] && is_palindrome(2,i-1)
    i-1 if s[i]==s[2] && is_palindrome(3,i-1)
    ...
    4   if s[i]==s[i-3] && is_palindrome(i-2,i-1)
    3   if s[i]==s[i-2] && is_palindrome(i-1,i-1)
    2   if s[i]==s[i-1]
    1
  }
  用一个memo[n][n]记录is_palindrome的结果
*/
string longestPalindrome(string s) {
  const int n = s.length();
  if (n == 0) return "";

  // memo[i][j]: s[i:j]是回文
  vector<vector<int>> memo(n);

  // 以 dp_pos 结尾的最长回文长度是 dp
  int dp_pos = 0, dp = 1;
  for (int i = 1; i < n; i++) {
    int dp_i = -1;
    for (int j : memo[i-1]) {
      if (s[i] == s[j-1]) {
        if (dp_i < 0) dp_i = i-j+2;
        if (0 < j-1) memo[i].push_back(j-1);  // 不放入0
      }
    }
    if (s[i] == s[i-1]) {
      dp_i = std::max(dp_i, 2);
      if (0 < i-1) memo[i].push_back(i-1);
    }
    memo[i].push_back(i);
    if (dp < dp_i) {
      dp_pos = i;
      dp = dp_i;
    }
  }
  return s.substr(dp_pos-dp+1, dp);
}

/*
  中心扩展法
*/
pair<int,int> search(const string& s, int n, int i, int j) {
  for (i--, j++; 0 <= i && j < n; i--, j++) {
    if (s[i] != s[j]) break;
  }
  i++, j--;
  return {i, j-i+1};  // s[i, j]是回文
}
string longestPalindrome_opt(string s) {
  const int n = s.length();
  if (n == 0) return "";

  int dp_pos = 0, dp = 1;
  for (int i = 0; i < n-1; i++) {
    auto dp_i = search(s, n, i, i);
    if (s[i] == s[i+1]) {
      auto dp_x = search(s, n, i, i+1);
      if (dp_i.second < dp_x.second) dp_i = dp_x;
    }
    if (dp < dp_i.second) {
      dp_pos = dp_i.first;
      dp = dp_i.second;
    }
  }
  return s.substr(dp_pos, dp);
}

int main() {
  print(longestPalindrome_opt("babad"));
}
