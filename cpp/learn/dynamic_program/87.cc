#include <header>

bool dfs(const string& s, int p, const string& t, int q, int n, unordered_map<uint64_t, bool>& memo) {
  if (n == 1) {
    return s[p] == t[q];
  }
  if (n == 2) {
    return (s[p] == t[q] && s[p+1] == t[q+1]) ||
           (s[p] == t[q+1] && s[p+1] == t[q]);
  }
  uint64_t key = (uint64_t(p) << 40) | (uint64_t(q) << 20) | uint64_t(n);
  auto iter = memo.find(key);
  if (iter != memo.end()) return iter->second;
  //
  for (int i = 1; i < n; i++) {
    if ((dfs(s, p, t, q, i, memo) && dfs(s, p+i, t, q+i, n-i, memo)) ||
        (dfs(s, p+i, t, q, n-i, memo) && dfs(s, p, t, q+(n-i), i, memo))) {
      memo[key] = true;
      return true;
    }
  }
  memo[key] = false;
  return false;
}
bool isScramble(string s, string target) {
  if (s.length() != target.length()) return false;
  if (s.empty()) return true;
  unordered_map<uint64_t, bool> memo;
  return dfs(s, 0, target, 0, (int)s.length(), memo);
}

/*
  memo改成数组后速度快了2倍(原先3倍)
*/
int memo_[32][32][32];
bool dfs(const string& s, int p, const string& t, int q, int n) {
  int& memo = memo_[p][q][n];
  if (memo != -1) return memo != 0;
  if (n == 1) return s[p] == t[q];
  if (n == 2) {
    return (s[p] == t[q] && s[p+1] == t[q+1]) ||
           (s[p] == t[q+1] && s[p+1] == t[q]);
  }
  //
  for (int i = 1; i < n; i++) {
    if ((dfs(s, p, t, q, i) && dfs(s, p+i, t, q+i, n-i)) ||
        (dfs(s, p+i, t, q, n-i) && dfs(s, p, t, q+(n-i), i))) {
      memo = 1;
      return true;
    }
  }
  memo = 0;
  return false;
}
bool isScramble(string s, string target) {
  if (s.length() != target.length()) return false;
  if (s.empty()) return true;
  for (int i = 0; i < 31; i++) {
    for (int j = 0; j < 31; j++)
      for (int k = 0; k < 31; k++)
        memo_[i][j][k] = -1;
  }
  return dfs(s, 0, target, 0, (int)s.length());
}

int main() {
  assert_T(isScramble("abcdbdacbdac", "bdacabcdbdac"));
  assert_F(isScramble("ccabcbabcbabbbbcbb", "bbbbabccccbbbabcba"));
}
