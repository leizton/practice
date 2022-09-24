#include <header>

/*
给定如下输入

dir \n
⟶ subdir1 \n
⟶ ⟶ file1.ext \n
⟶ ⟶ subsubdir1 \n
⟶ subdir2 \n
⟶ ⟶ subsubdir2 \n
⟶ ⟶ ⟶ file2.ext \n

一个文件绝对路径是 dir/subdir1/file1.ext

返回文件系统中指向`文件`的`最长绝对路径`的长度
*/

struct Node {
  int path_len;
  int level;  // 第几层目录, 根目录是第0层

  Node(int _1, int _2)
      : path_len(_1), level(_2) {}
};
int lengthLongestPath(string input) {
  const int len = (int)input.length();
  const char* s = input.c_str();
  vector<Node> stk;
  stk.reserve(8);
  stk.emplace_back(-1, -1);

  int res = 0;
  for (int i = 0; i < len; ) {
    int j = i;
    while (s[j] == '\t') j++;
    int level = j - i;
    i = j;
    while (j < len && s[j] != '\n' && s[j] != '.') j++;
    bool is_file = false;
    if (s[j] == '.') {
      is_file = true;
      while (j < len && s[j] != '\n') j++;
    }
    //
    while (level <= stk.back().level) stk.pop_back();
    int path_len = stk.back().path_len + 1 + j - i;
    if (is_file) res = std::max(res, path_len);
    else stk.emplace_back(path_len, level);
    i = j + 1;
  }
  return res;
}
