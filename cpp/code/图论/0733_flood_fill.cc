#include <util/base.h>

vector<vector<int>> floodFill(vector<vector<int>>& img, int sr, int sc, int new_color) {
  if (img.empty() || img[0].empty()) return img;
  const int row = img.size(), col = img[0].size();
  const int row1 = row - 1, col1 = col - 1;
  if (sr >= row || sc >= col) return img;

  const int old_color = img[sr][sc];
  if (old_color == new_color) return img;
  img[sr][sc] = new_color;

  #define explore(i, j) {\
    if (img[i][j] == old_color) { \
      img[i][j] = new_color; \
      q.emplace(i, j); \
    }}

  queue<pair<int, int>> q;
  q.emplace(sr, sc);
  while (!q.empty()) {
    sr = q.front().first;
    sc = q.front().second;
    q.pop();
    if (sr > 0)    explore(sr-1, sc); // up
    if (sr < row1) explore(sr+1, sc); // down
    if (sc > 0)    explore(sr, sc-1); // left
    if (sc < col1) explore(sr, sc+1); // right
  }

  return img;
}

int main() {
  vector<vector<int>> img(3);
  img[0] = {1,1,1};
  img[1] = {1,1,0};
  img[2] = {1,0,1};
  floodFill(img, 1, 1, 2);
  print(img);
}
