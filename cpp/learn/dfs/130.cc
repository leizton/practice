#include <header>

/*
给你一个 m x n 的矩阵 board ，由若干字符 'X' 和 'O'
找到所有被 'X' 围绕的区域，并将这些区域里所有的 'O' 用 'X' 填充
*/

void dfs(vector<vector<char>>& board, int i, int j) {
  // 标记成 live
  board[i][j] = 'o';
  if (i > 0) {
    if (board[i-1][j] == 'O') dfs(board, i-1, j);
  }
  if (i < board.size()-1) {
    if (board[i+1][j] == 'O') dfs(board, i+1, j);
  }
  if (j > 0) {
    if (board[i][j-1] == 'O') dfs(board, i, j-1);
  }
  if (j < board[0].size()-1) {
    if (board[i][j+1] == 'O') dfs(board, i, j+1);
  }
}
void solve(vector<vector<char>>& board) {
  if (board.empty() || board[0].empty()) return;
  int m = board.size(), n = board[0].size();
  // 只有边缘的'O'不被包围
  for (int j = 0; j < n; j++) {
    if (board[0][j] == 'O') dfs(board, 0, j);
    if (board[m-1][j] == 'O') dfs(board, m-1, j);
  }
  for (int i = 0; i < m; i++) {
    if (board[i][0] == 'O') dfs(board, i, 0);
    if (board[i][n-1] == 'O') dfs(board, i, n-1);
  }
  //
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == 'O') board[i][j] = 'X';
    }
  }
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == 'o') board[i][j] = 'O';
    }
  }
}
