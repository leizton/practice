#include <header>

/*
给你一个字符串 s 和一个字符串列表 wordDict 作为字典。请你判断是否可以利用字典中出现的单词拼接出 s 。
注意：不要求字典中出现的单词全部都使用，并且字典中的单词可以重复使用。

输入: s = "applepenapple", wordDict = ["apple", "pen"]
输出: true
解释: 返回 true 因为 "applepenapple" 可以由 "apple" "pen" "apple" 拼接成
*/

class WordPrefixTree {
private:
  struct Node {
    Node* next = nullptr; // next非空表示当前node是某些单词的一个字符
    bool is_end = false;  // next非空也有可能是一个单词的结束
    ~Node() { if (next) delete[] next; }
  };
  Node root_;

public:
  WordPrefixTree() {
    root_.next = new Node[26];
  }

  void addWord(const string& w) {
    Node* prev = &root_;
    for (int i = 0; i < (int)w.length(); i++) {
      Node& curr = prev->next[int(w[i] - 'a')];
      if (curr.next == nullptr) {
        curr.next = new Node[26];
      }
      prev = &curr;
    }
    prev->is_end = true;
  }

  vector<int> searchWords(const string& s, int begin) const {
    vector<int> word_end;
    const Node* prev = &root_;
    for (int i = begin; i < (int)s.length(); i++) {
      const Node& curr = prev->next[int(s[i] - 'a')];
      if (curr.next == nullptr) break;
      if (curr.is_end) word_end.push_back(i+1);
      prev = &curr;
    }
    return word_end;
  }
};

// memo记录是否已经检查过
bool dfs(const string& s, int begin, const WordPrefixTree& wtree, vector<uint8_t>& memo) {
  if (begin == (int)s.length()) return true;
  if (memo[begin]) return false;
  auto curr_word_end = wtree.searchWords(s, begin);
  for (int end : curr_word_end) {
    if (dfs(s, end, wtree, memo)) return true;
  }
  memo[begin] = 1;
  return false;
}
bool wordBreak_139(const string& s, vector<string>& words) {
  WordPrefixTree wtree;
  for (auto& w : words) {
    wtree.addWord(w);
  }
  vector<uint8_t> memo(s.length(), 0);  // 记录是否已经检查过
  return dfs(s, 0, wtree, memo);
}

bool wordBreak_139_official(const string& s, vector<string>& words) {
  unordered_set<string> word_set;
  for (auto& w : words) {
    word_set.insert(w);
  }

  // dp[i] 表示 s[0,i) 是否可以由word_set的单词组成
  vector<uint8_t> dp(s.length() + 1, 0);
  dp[0] = 1;
  for (int i = 1; i <= (int)s.length(); i++) {
    for (int j = 0; j < i; j++) {
      // word_set.contains(s[j, i-1]) 表示 s[j,i-1]满足wordBreak条件
      if (dp[j] && word_set.find(s.substr(j, i-j)) != word_set.end()) {
        dp[i] = 1;
        break;
      }
    }
  }
  return dp[s.length()];
}

/*
140.cc
返回 139.cc 中所有的可能分割结果

输入:s = "catsanddog", wordDict = ["cat","cats","and","sand","dog"]
输出:["cats and dog","cat sand dog"]
*/

/*
  动规模板之《序列的子串分割》
*/
void dfs_build(const string& s, vector<vector<int>>& gh, vector<int>& path, vector<string>& ret) {
  int curr_node = path.back();
  if (curr_node < 0) {
    ret.emplace_back();
    auto& res = ret.back();
    res.reserve(s.size() + path.size());
    for (int i = path.size()-1; i >= 1; i--) {
      res += s.substr(path[i] + 1, path[i-1] - path[i]) + " ";
    }
    res.resize(res.size() - 1);
    return;
  }
  for (int prev_node : gh[curr_node]) {
    path.push_back(prev_node);
    dfs_build(s, gh, path, ret);
    path.pop_back();
  }
}
vector<string> wordBreak_140(const string& s, vector<string>& words) {
  const int n = (int)s.length();
  if (n == 0) return vector<string>();

  unordered_set<string> word_set;
  for (auto& w : words) {
    word_set.insert(w);
  }

  // gh[i][j-1] 表示 s[j,i] 是 word_set 中的单词 且 s[0,j-1]可拆
  vector<vector<int>> gh(n);
  if (word_set.find(s.substr(0, 1)) != word_set.end()) {
    gh[0].push_back(-1);
  }
  for (int i = 1; i < n; i++) {
    if (word_set.find(s.substr(0, i+1)) != word_set.end()) {
      gh[i].push_back(-1);
    }
    for (int j = 1; j <= i; j++) {
      if (!gh[j-1].empty() && word_set.find(s.substr(j, i-j+1)) != word_set.end()) {
        gh[i].push_back(j-1);
      }
    }
  }

  vector<string> ret;
  vector<int> path;
  path.push_back(n-1);
  dfs_build(s, gh, path, ret);
  return ret;
}

int main() {
  {
    string s = "applepenapple";
    vector<string> words{"apple", "pen"};
    assert_T(wordBreak_139(s, words));
  }
  {
    string s = "aaaaab";
    vector<string> words{"a","aa"};
    assert_F(wordBreak_139(s, words));
  }
  {
    string s = "catsanddog";
    vector<string> words{"cat","cats","and","sand","dog"};
    vector<string> answer{"cat sand dog", "cats and dog"};
    assert_T(wordBreak_139(s, words));
    assert_eq(answer, wordBreak_140(s, words));
  }
}
