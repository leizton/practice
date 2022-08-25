#include <util/base.h>

/*
给定一个字符串 (s) 和一个字符模式 (p) ，实现一个支持 '?' 和 '*' 的通配符匹配。
两个字符串完全匹配才算匹配成功。
*/

bool is_match_memo(const string& s, size_t s_begin, const string& p, size_t p_begin, vector<vector<int8_t>>& memo) {
  #define memo_match(s, si, p, pj) \
    int8_t tmp = memo[si][pj]; \
    { if (tmp == -1) memo[si][pj] = tmp = is_match_memo(s, si, p, pj, memo) ? 1 : 0; }
  //
  if (p_begin < p.length()) {
    if (p[p_begin] == '*') {
      if (p_begin == p.length() - 1) return true;
      if (s_begin < s.length()) {
        for (size_t i = s_begin; i < s.length(); i++) {
          memo_match(s, i, p, p_begin+1);
          if (tmp == 1) return true;
        }
        return false;
      } else {
        for (size_t i = p_begin+1; i < p.length(); i++) {
          if (p[i] != '*') return false;
        }
        return true;
      }
    } else {
      if (s_begin < s.length()) {
        if (p[p_begin] != '?' && p[p_begin] != s[s_begin]) return false;
        memo_match(s, s_begin+1, p, p_begin+1);
        return tmp == 1;
      } else {
        return false;
      }
    }
  } else {
    return s.length() <= s_begin;
  }
}

bool isMatchMemo(string s, string p) {
  if (p.empty()) {
    return s.empty();
  }
  if (s.empty()) {
    for (size_t i = 0; i < p.length(); i++) {
      if (p[i] != '*') return false;
    }
    return true;
  }
  vector<vector<int8_t>> memo;
  memo.resize(s.length() + 1);
  for (size_t i = 0; i < memo.size(); i++) {
    memo[i].resize(p.length() + 1, -1);
  }
  return is_match_memo(s, 0, p, 0, memo);
}

// DP on Strings
bool isMatchDP(string s, string p) {
  if (p.empty()) {
    return s.empty();
  }
  if (s.empty()) {
    for (size_t i = 0; i < p.length(); i++) {
      if (p[i] != '*') return false;
    }
    return true;
  }
  //
  vector<vector<bool>> dp;
  dp.resize(p.length());
  for (size_t i = 0; i < p.length(); i++) {
    dp[i].resize(s.length(), false);
  }
  //
  if (p[0] == '*') {
    for (size_t si = 0; si < s.length(); si++) dp[0][si] = true;
  } else {
    dp[0][0] = (p[0] == '?' || p[0] == s[0]);
  }
  if (dp[0][0]) {
    for (size_t pj = 1; pj < p.length(); pj++) {
      if (p[pj] == '*') {
        dp[pj][0] = true;
      } else {
        dp[pj][0] = (p[0] == '*') && (p[pj] == '?' || p[pj] == s[0]);
        break;
      }
    }
  }
  //
  for (size_t pj = 1; pj < p.length(); pj++) {
    auto& prev = dp[pj-1];
    auto& curr = dp[pj];
    if (p[pj] == '*') {
      for (size_t si = 0; si < s.length(); si++) {
        if (prev[si]) {
          for (; si < s.length(); si++) curr[si] = true;
          break;
        }
      }
    } else {
      for (size_t si = 1; si < s.length(); si++) {
        curr[si] = (p[pj] == '?' || p[pj] == s[si]) && prev[si-1];
      }
    }
  }
  return dp[p.length()-1][s.length()-1];
}

int main() {
  std::function<bool(string,string)> test = isMatchDP;
  assert_F(test("aa", "a"));
  assert_T(test("aa", "*"));
  assert_F(test("cb", "?a"));
  assert_T(test("adceb", "*a*b"));
  assert_F(test("acdcb", "a*c?b"));
  assert_F(test("mississippi", "m??*ss*?i*pi"));
}
