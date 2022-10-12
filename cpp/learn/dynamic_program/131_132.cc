#include <header>

/*
给你一个字符串 s，请你将 s 分割成一些子串，使每个子串都是回文串
返回 s 所有可能的分割方案

输入：s = "aab"
输出：[["a","a","b"],["aa","b"]]
*/

/*
  分治思想的遍历
*/
bool isPalindrome(const string& s, int left, int right) {
  while (left < right) if (s[left++] != s[right--]) return false;
  return true;
}
void dfs(const string& s, int begin, vector<Pair>& prev_path, vector<vector<Pair>>& ret) {
  // prev_path是[0,begin-1]上的某个结果
  if (begin == (int)s.length()) {
    ret.emplace_back(prev_path);
    return;
  }
  //
  for (int i = begin; i < (int)s.length(); i++) {
    if (isPalindrome(s, begin, i)) {
      prev_path.emplace_back(begin, i+1-begin);
      dfs(s, i+1, prev_path, ret);  // 继续处理剩余的[i+1,n-1]
      prev_path.pop_back();  // 回溯
    }
  }
}
vector<vector<string>> partition_131_dfs(string s) {
  vector<vector<Pair>> ret;
  vector<Pair> prev_path;
  prev_path.reserve(s.length());
  dfs(s, 0, prev_path, ret);
  return convertResult(s, ret);
}

/*
  动规模板之《序列的子串分割》
*/
void dfs_build(const string& s, vector<vector<int>>& gh, vector<int>& path, vector<vector<string>>& ret) {
  int curr_node = path.back();
  if (curr_node < 0) {
    ret.emplace_back();
    auto& res = ret.back();
    res.reserve(path.size());
    for (int i = path.size()-1; i >= 1; i--)
      res.push_back(s.substr(path[i] + 1, path[i-1] - path[i]));
    return;
  }
  for (int prev_node : gh[curr_node]) {
    path.push_back(prev_node);
    dfs_build(s, gh, path, ret);
    path.pop_back();
  }
}
vector<vector<string>> partition_131(string s) {
  const int n = (int)s.length();
  if (n == 0) return vector<vector<string>>{{""}};

  vector<vector<uint8_t>> is_palindrome(n, vector<uint8_t>(n, 1));
  for (int i = 1; i < n; i++)
    for (int j = i-1; j >= 0; j--)
      is_palindrome[j][i] = (s[j] == s[i]) && is_palindrome[j+1][i-1];

  // gh[i][j-1] 表示 s[j,i] 构成回文 且 s[0,j-1]可拆
  vector<vector<int>> gh(n);
  gh[0].push_back(-1);  // s[0]自身构成回文
  for (int i = 1; i < n; i++) {
    if (is_palindrome[0][i]) {
      gh[i].push_back(-1);
    }
    for (int j = 1; j <= i; j++) {
      if (!gh[j-1].empty() && is_palindrome[j][i]) {
        gh[i].push_back(j-1);
      }
    }
  }

  // transform
  vector<vector<string>> ret;
  vector<int> path;
  path.push_back(n-1);
  dfs_build(s, gh, path, ret);
  return ret;
}

int main() {
  print("--------------------");
  print(partition(""));
  print("--------------------");
  print(partition("a"));
  print("--------------------");
  print(partition("aa"));
  print("--------------------");
  print(partition("aab"));
}

/*
  132.cc

  给你一个字符串 s，请你将 s 分割成一些子串，使每个子串都是回文。
  返回符合要求的 最少分割次数

  输入：s = "aab"
  输出：1
  解释：只需一次分割就可将 s 分割成 ["aa","b"] 这样两个回文子串。
*/
int minCut_132(string s) {
  const int n = (int)s.length();
  if (n <= 1) return 0;

  vector<vector<uint8_t>> is_palindrome(n, vector<uint8_t>(n, 1));
  for (int i = 1; i < n; i++)
    for (int j = i-1; j >= 0; j--)
      is_palindrome[j][i] = (s[j] == s[i]) && is_palindrome[j+1][i-1];

  // dp[i]表示[0,i]上的最小分割数
  vector<int> dp(n, 0);
  for (int i = 1; i < (int)n; i++) {
    if (is_palindrome[0][i]) {
      dp[i] = 0;
      continue;
    }
    int cut_num = INT_MAX;
    for (int from = 1; from <= i; from++) {
      if (is_palindrome[from][i]) {
        int cut = dp[from-1] + 1;
        cut_num = std::min(cut_num, cut);
      }
    }
    dp[i] = cut_num;
  }
  return dp[n-1];
}
