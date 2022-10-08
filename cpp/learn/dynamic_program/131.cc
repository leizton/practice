#include <header>

/*
给你一个字符串 s，请你将 s 分割成一些子串，使每个子串都是回文串
返回 s 所有可能的分割方案

输入：s = "aab"
输出：[["a","a","b"],["aa","b"]]
*/

/*
  类似 122.cc
  两个状态: s[i]前面 有分割符 和 没有分割符
*/
struct Pair {
  int begin; int num;
  Pair() : begin(-1), num(0) {}
  Pair(int b, int n) : begin(b), num(n) {}
};
vector<vector<string>> convertResult(const string& s, const vector<vector<Pair>>& input) {
  vector<vector<string>> ret;
  ret.reserve(input.size());
  for (const vector<Pair>& e : input) {
    ret.push_back({});
    auto& r = ret.back();
    r.reserve(e.size());
    for (const Pair& p : e) {
      r.emplace_back(s, p.begin, p.num);
    }
  }
  return ret;
}
vector<vector<string>> partition_v1(string s) {
  if (s.empty()) vector<vector<string>>{{""}};
  vector<vector<Pair>> ret;
  ret.push_back({Pair(0, 1)});

  const int len = (int)s.length();
  vector<vector<pair<int,int>>> split_mapping(len, vector<pair<int,int>>());
  vector<vector<uint8_t>> is_palindrome(len, vector<uint8_t>(len, 1));
  for (int i = 1; i < len; i++)
    for (int j = i-1; j >= 0; j--)
      is_palindrome[j][i] = (s[j] == s[i]) && is_palindrome[j+1][i-1];

  for (int i = 1; i < (int)len; i++) {
    const int prev_size = (int)ret.size();

    // 状态-1 s[i]前没有分割. s[i]需要和前面的子串构成回文
    if (is_palindrome[0][i]) {
      ret.push_back(vector<Pair>{Pair{0, i+1}});
    }
    for (int from = 1; from < i; from++) {
      if (is_palindrome[from][i]) {
        vector<pair<int,int>>& cands = split_mapping[from];
        for (auto& cand : cands) {
          auto prev_path = ret[cand.first].begin();  // [0, from-1]上的结果
          ret.push_back(vector<Pair>{prev_path, prev_path + cand.second});
          ret.back().push_back(Pair{from, i+1-from});
        }
      }
    }

    // 状态-2 s[i]前有分割. s[i]是独立的字符
    split_mapping[i].reserve(prev_size);
    for (int j = 0; j < prev_size; j++) {
      split_mapping[i].emplace_back(j, (int)ret[j].size());
      ret[j].push_back(Pair{i, 1});
    }
  }
  return convertResult(s, ret);
}

/*
  分治思想的遍历
*/
bool isPalindrome(const string& s, int left, int right) {
  while (left < right) if (s[left++] != s[right--]) return false;
  return true;
}
void dfs(const string& s, int begin, vector<Pair>& prev_path, vector<vector<Pair>>& ret) {
  // prev_path是[0,begin-1]上的某个结果
  if (begin == s.length()) {
    ret.emplace_back(prev_path);
    return;
  }
  //
  for (int i = begin; i < s.length(); i++) {
    if (isPalindrome(s, begin, i)) {
      prev_path.emplace_back(begin, i+1-begin);
      dfs(s, i+1, prev_path, ret);  // 继续处理剩余的[i+1,len-1]
      prev_path.pop_back();  // 回溯
    }
  }
}
vector<vector<string>> partition(string s) {
  if (s.empty()) vector<vector<string>>{{""}};
  vector<vector<Pair>> ret;
  vector<Pair> prev_path;
  prev_path.reserve(s.length());
  dfs(s, 0, prev_path, ret);
  return convertResult(s, ret);
}
