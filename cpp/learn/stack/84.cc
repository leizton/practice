#include <util/base.h>

/*
给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为1
求在该柱状图中，能够勾勒出来的矩形的最大面积
*/

/*
  单调栈
  不小于当前高度h[i]的左边界和右边界位置
  接雨水(42.cc)只需要知道左右两边高度的最大值, 不需要知道位置
*/
int largestRectangleArea(vector<int>& h) {
  if (h.empty()) return 0;
  vector<pair<int,int>> sk;  // pair<pos, left_bound>
  sk.emplace_back(0, 0);
  int ans = 0;
  for (int i = 1; i < h.size(); i++) {
    int left_bound_i = i;
    // try pop
    int right_bound = sk.back().first;
    while (!sk.empty()) {
      int pos = sk.back().first, left_bound = sk.back().second;
      if (h[i] <= h[pos]) {
        left_bound_i = left_bound;  // 继承左边界
        ans = std::max(ans, (right_bound - left_bound + 1) * h[pos]);
        sk.pop_back();
      } else {
        break;
      }
    }
    //
    sk.emplace_back(i, left_bound_i);
  }
  int right_bound = sk.back().first;
  while (!sk.empty()) {
    int pos = sk.back().first, left_bound = sk.back().second;
    ans = std::max(ans, (right_bound - left_bound + 1) * h[pos]);
    sk.pop_back();
  }
  return ans;
}
