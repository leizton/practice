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

bool dfs(const string& s, int begin, const WordPrefixTree& wtree, vector<uint8_t>& checked) {
  if (begin == (int)s.length()) return true;
  if (checked[begin]) return false;
  auto curr_word_end = wtree.searchWords(s, begin);
  for (int end : curr_word_end) {
    if (dfs(s, end, wtree, checked)) return true;
  }
  checked[begin] = 1;
  return false;
}

// @ref 131.cc
bool wordBreak(const string& s, vector<string>& words) {
  WordPrefixTree wtree;
  for (auto& w : words) {
    wtree.addWord(w);
  }
  vector<uint8_t> checked(s.length(), 0);
  return dfs(s, 0, wtree, checked);
}

bool wordBreak_official(const string& s, vector<string>& words) {
  unordered_set<string> word_set;
  for (auto& w : words) {
    word_set.insert(w);
  }

  // dp[i] 表示 s[0,i) 是否可以由word_set的单词组成
  vector<uint8_t> dp(s.length() + 1, 0);
  dp[0] = 1;
  for (int i = 1; i <= (int)s.length(); i++) {
    for (int j = 0; j < i; j++) {
      if (dp[j] && word_set.find(s.substr(j, i-j)) != word_set.end()) {
        dp[i] = 1;
        break;
      }
    }
  }
  return dp[s.length()];
}

int main() {
  {
    string s = "applepenapple";
    vector<string> words{"apple", "pen"};
    assert_T(wordBreak(s, words));
  }
  {
    string s = "aaaaab";
    vector<string> words{"a","aa"};
    assert_F(wordBreak(s, words));
  }
}
