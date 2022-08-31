#include <util/base.h>

/*
数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的且有效的括号组合
1 <= n <= 8

输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
*/

void dfs(char* s, int prev_ln, int prev_n, int n, vector<string>& out) {
  if (prev_n == n) {
    // 此时无需检查s是否成立
    // 因为每次加入右括号时, 如果左边一个字符恰好是左括号, 则两个字符匹配并消消乐
    // 经过上面的消消乐后, 每次加入右括号时都相当于s中只有左括号, 所以每次都能消消乐
    out.emplace_back(s);
    return;
  }
  // 下一个位置有两个选择: 左括号 右括号, 是一颗二叉树
  if (prev_ln < (n >> 1)) {
    s[prev_n] = '(';
    dfs(s, prev_ln+1, prev_n+1, n, out);
  }
  // 右括号数 小于 左括号数, 则当前可以放入一个右括号
  if ((prev_n - prev_ln) < prev_ln) {
    s[prev_n] = ')';
    dfs(s, prev_ln, prev_n+1, n, out);
  }
}

vector<string> generateParenthesis(int n) {
  n <<= 1;
  char* s = new char[n+2];
  s[n] = '\0';
  vector<string> ans;
  dfs(s, 0, 0, n, ans);
  delete[] s;
  return ans;
}

int main() {
  auto test = [] (int n) {
    auto ans = generateParenthesis(n);
    return ans.size();
  };
  for (int i = 1; i <= 8; i++) {
    print(test(i));
  }
}
