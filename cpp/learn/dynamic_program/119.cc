#include <header>

/*
给定一个非负索引 row, 返回「杨辉三角」的第 row 行
row 从0开始
*/

vector<int> getRow(int row) {
  if (row < 0) return vector<int>{};
  if (row == 0) return vector<int>{1};
  if (row == 1) return vector<int>{1, 1};
  vector<int> curr(row+1, 1), next(row+1, 1);
  for (int ri = 2; ri <= row; ri++) {
    for (int i = 1; i < ri; i++) {
      next[i] = curr[i-1] + curr[i];
    }
    curr.swap(next);
  }
  return curr;
}

int main() {
  vector<int> ans{1,3,3,1};
  assert_eq(getRow(3), ans);
}
