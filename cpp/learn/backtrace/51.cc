#include <util/base.h>

/*
N-皇后

没有形成攻击的一种情形, 如下
- - Q -
Q - - -
- - - Q
- Q - -

*/

void backtrace(vector<string>& curr_path, vector<vector<int8_t>>& state, vector<vector<string>>& res) {
  const int N = state.size();
  if ((int)curr_path.size() == N) {
    res.push_back(curr_path);
    return;
  }
  int row = (int)curr_path.size();
  int i = 0, j = 0;
  string row_str(N, '.');
  for (int col = 0; col < N; col++) {
    bool ok = true;
    #define CHECK_STATE { if (state[i][j] != 0) { ok = false; break; } } if (!ok) continue;
    for (i = 0, j = col; i < N; i++) CHECK_STATE;
    for (i = row-1, j = col-1; i >= 0 && j >= 0; i--, j--) CHECK_STATE;
    for (i = row-1, j = col+1; i >= 0 && j <  N; i--, j++) CHECK_STATE;
    for (i = row+1, j = col-1; i <  N && j >= 0; i++, j--) CHECK_STATE;
    for (i = row+1, j = col+1; i <  N && j <  N; i++, j++) CHECK_STATE;
    //
    state[row][col] = 1;
    row_str[col] = 'Q';
    curr_path.push_back(row_str);
    backtrace(curr_path, state, res);
    curr_path.pop_back();
    row_str[col] = '.';
    state[row][col] = 0;
  }
}

vector<vector<string>> solveNQueens(int n) {
  vector<vector<int8_t>> state(n);
  for (auto& row : state) {
    row.resize(n, 0);
  }

  vector<vector<string>> res;
  vector<string> curr_path;
  curr_path.reserve(n);
  backtrace(curr_path, state, res);
  return res;
}
