#include <util/base.h>

/*
给你一个字符串 s，找到 s 中最长的回文子串
*/

/*
  if match(s[i+1..j-1]) && s[i] = s[j]
    s[i..j] = true
  else
    s[i..j] = false
*/

string longestPalindrome0(string s) {
  if (s.empty()) return "";
  const int N = s.length();
  vector<vector<bool>> dp(N, vector<bool>(N, false));
  int ans_from = 0, ans_to = 0;
  // dp[i][i]
  for (int i = 0; i < N; i++) {
    dp[i][i] = true;
    ans_from = ans_to = i;
  }
  // dp[i][i+1]
  for (int i = 0; i < N-1; i++) {
    if (s[i] == s[i+1]) {
      dp[i][i+1] = true;
      ans_from = i;
      ans_to = i+1;
    }
  }
  //
  for (int k = 2; k < N; k++) {
    int j = k;
    for (int i = 0; i < N-k; i++,j++) {
      if (s[i] == s[j] && dp[i+1][j-1]) {
        dp[i][j] = true;
        ans_from = i;
        ans_to = j;
      }
    }
  }
  return s.substr(ans_from, ans_to-ans_from+1);
}

/*
  改变推进方式, 实现提前剪枝
*/
int search(const char* s, int N, int i, int j, int& from) {
  for (i--, j++; 0 <= i && j < N; i--, j++) {
    // 提前剪枝
    if (s[i] != s[j]) break;
  }
  from = i+1;
  return (j-1) - from;
}
string longestPalindrome(string s) {
  if (s.empty()) return "";
  const int N = s.length();
  const char* sc = s.c_str();
  int ans_from = 0, longest = 0;
  int from = 0;
  // dp[i][i]
  for (int i = 1; i < N-1; i++) {
    int cand = search(sc, N, i, i, from);
    if (longest < cand) {
      longest = cand;
      ans_from = from;
    }
  }
  // dp[i][i+1]
  for (int i = 0; i < N-1; i++) {
    if (sc[i] != sc[i+1]) continue;
    int cand = search(sc, N, i, i+1, from);
    if (longest < cand) {
      longest = cand;
      ans_from = from;
    }
  }
  return s.substr(ans_from, longest+1);
}

int main() {
  auto test = longestPalindrome;
  assert_eq("aba", test("caba"));
  assert_eq("xaabacxcabaax", test("xaabacxcabaaxcabaax"));
}
