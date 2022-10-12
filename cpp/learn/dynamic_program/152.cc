#include <header>

/*
给你一个整数数组 nums
请你找出数组中乘积最大的非空连续子数组
该子数组中`至少包含一个数字`
并返回该子数组所对应的乘积

输入: nums = [2,3,-2,4]
输出: 6
解释: 子数组 [2,3] 有最大乘积 6
*/

int maxProduct(vector<int>& nums) {
  int n = nums.size();
  if (n == 0) return 0;

  // dp[i] 以i结尾的子串的最大乘积
  // dp_neg[i] 以i结尾的子串的最小负数乘积
  vector<int> dp(n), dp_neg(n);
  dp[0] = dp_neg[0] = nums[0];
  int max_prod = dp[0];
  for (int i = 1; i < n; i++) {
    int v0 = nums[i];
    int v1 = v0 * dp[i-1];
    int v2 = v0 * dp_neg[i-1];
    if (v0 < 0) {
      dp[i] = std::max(v0, ((dp_neg[i-1] < 0) ? v2 : v1));
      dp_neg[i] = std::min(v0, v1);
    } else if (v0 == 0) {
      dp[i] = dp_neg[i] = 0;
    } else {
      dp[i] = std::max(v0, v1);
      dp_neg[i] = (dp_neg[i-1] < 0) ? v2 : 0;
    }
    max_prod = std::max(max_prod, dp[i]);
  }
  return max_prod;
}

int main() {
  {
    vector<int> v{2, 3, -2, 4};
    assert_eq(6, maxProduct(v));
  }
}
