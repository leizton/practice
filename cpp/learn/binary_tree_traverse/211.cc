#include <header>

/*
实现词典类 WordDictionary ：
  void addWord(word)
      将 word 添加到数据结构中，之后可以对它进行匹配
  bool search(word)
      如果数据结构中存在字符串与 word 匹配, 则返回true, 否则返回false
      word 中可能包含 '.' ，每个 . 都可以表示任何一个字母。
*/

class WordDictionary {
private:
  struct Node {
    Node* next = nullptr;
    bool is_end = false; // next非空也有可能是一个单词的结束
    ~Node() { if (next) delete[] next; }
  };
  Node root_;

public:
  WordDictionary() {
    root_.next = new Node[26];
  }

  void addWord(const string& w) {
    Node* prev = &root_;
    for (int i = 0; i < w.length(); i++) {
      Node& curr = prev->next[int(w[i] - 'a')];
      if (curr.next == nullptr) {
        curr.next = new Node[26];
      }
      prev = &curr;
    }
    prev->is_end = true;
  }

  bool search(const string& w) {
    return search(w.c_str(), 0, w.length(), &root_);
  }

private:
  bool search(const char* w, int begin, int end, Node* prev) {
    for (int i = begin; i < end; i++) {
      if (w[i] == '.') {
        for (int c = 0; c < 26; c++) {
          Node& curr = prev->next[c];
          if (curr.next && search(w, i+1, end, &curr)) return true;
        }
        return false;
      } else {
        Node& curr = prev->next[int(w[i] - 'a')];
        if (curr.next == nullptr) return false;
        prev = &curr;
      }
    }
    return prev->is_end;
  }
};