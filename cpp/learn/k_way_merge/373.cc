#include <header>

vector<vector<int>> kSmallestPairs(vector<int>& a1, vector<int>& a2, int kth) {
  int m = int(a1.size()), n = int(a2.size());
  if (m == 0 || n == 0) return {};
  m = min(kth,m), n = min(kth,n);
  vector<vector<int>> ans(kth, {0, 0});

  using E = tuple<int,int,int>;
  auto cmp = [] (const E& x, const E& y) { return get<0>(x) > get<0>(y); };
  priority_queue<E, vector<E>, decltype(cmp)> que;

  // 多路归并
  // 第0路: a1[0]+a2[0], a1[0]+a2[1], a1[0]+a2[2], ...
  // 第1路: a1[1]+a2[0], a1[1]+a2[1], a1[1]+a2[2], ...
  // 第m-1路: a1[m-1]+a2[0], a1[m-1]+a2[1], a1[m-1]+a2[2], ...
  for (int i = 0; i < m; i++) {
    // 每路第一个元素入堆
    que.emplace(a1[i]+a2[0], i, 0);
  }

  int sum, i, j;
  for (int k = 0; k < kth && !que.empty(); k++) {
    tie(sum, i, j) = que.top();
    que.pop();
    ans[k][0]=a1[i], ans[k][1]=a2[j];
    // 第i路下一个元素入堆
    if (j+1 < n) que.emplace(a1[i]+a2[j+1], i, j+1);
  }

  return ans;
}
