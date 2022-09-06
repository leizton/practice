#include <util/base.h>

/*
给你一个整数数组nums，找到其中最长严格递增子序列的长度。
子序列是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。例如，[3,6,2,7] 是数组 [0,3,1,6,2,2,7] 的子序列。
*/

/*
  设 dp[i] 是以 nums[i] 结尾的最长递增子序列的长度
*/
int lengthOfLIS(vector<int>& nums) {
  if (nums.empty()) return 0;
  const int n = nums.size();
  vector<int> dp(n, 0);
  dp[0] = 1;
  int ans = 1;
  //
  for (int i = 1; i < n; i++) {
    int dp_i = 1;
    for (int j = 0; j < i; j++) {
      if (nums[j] < nums[i]) {
        dp_i = std::max(dp_i, dp[j]+1);
      }
    }
    dp[i] = dp_i;
    ans = std::max(ans, dp[i]);
  }
  return ans;
}

int main() {
  auto test = [] (vector<int> nums) {
    return lengthOfLIS(nums);
  };
  assert_eq(4, test({0,3,1,6,2,2,7}));
  assert_eq(4, test({10,9,2,5,3,7,101,18}));
}
