#include <util/base.h>

/*
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。
*/

/**
 * 找区间 [i,j], i<j-1 and h[i+1,j-1] < min(h_i, h_j)
 * 以上区间再做合并. 合并只是相邻区间 例如[i,j]和[j,k], [i,j+p]和[j-q,k]不会出现
 * 
 * 对于位置k 分别找[0,k-1]、[k+1,N)上最大值l_max、r_max 即是k所处的(合并后)区间
 * 这个区间上位置k的存水量是min(l_max,r_max)-h_k
*/

/**
 * 当前位置能存多少取决于左右两边的max
*/
int trap(vector<int>& h) {
  if (h.size() <= 2) return 0;
  const int N = h.size();
  //
  vector<int> left_max(N);
  int tmp = h[0];
  for (int i = 1; i < N; i++) {
    tmp = std::max(tmp, h[i]);
    left_max[i] = tmp;
  }
  vector<int> right_max(N);
  tmp = h[N-1];
  for (int i = N-2; i >= 0; i--) {
    tmp = std::max(tmp, h[i]);
    right_max[i] = tmp;
  }
  //
  int ans = 0;
  for (int i = 1; i < N-1; i++) {
    ans += std::min(left_max[i], right_max[i]) - h[i];
  }
  return ans;
}

int main() {
  auto test = [](vector<int> h) {
    return trap(h);
  };
  assert_eq(6, test({0,1,0,2,1,0,1,3,2,1,2,1}));
  assert_eq(13, test({5,2,0,3,2,5}));
}
