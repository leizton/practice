#include <header>

class WordDictionary {
private:
  struct Node {
    Node* next = nullptr;
    bool is_end = false;

    ~Node() {
      if (next) {
        delete[] next;
        next = nullptr;
      }
    }
  };
  Node root_;

public:
  WordDictionary() {
    root_.next = new Node[26];
  }

  void addWord(string w) {
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

  bool search(string w) {
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