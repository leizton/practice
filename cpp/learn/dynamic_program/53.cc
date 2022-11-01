#include <util/base.h>

/*
给你一个整数数组 nums ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和
*/

int maxSubArray(vector<int>& nums) {
  // dp[i]: 以i作终点的连续子数组的最大值
  /*
  dp[0] = nums[0];
  for (int i = 1; i < n; i++) {
    dp[i] = std::max(dp[i-1]+nums[i], nums[i]);
  }
  */

  int max_sum = INT32_MIN, dp = 0;
  for (int num : nums) {
    dp = std::max(dp+num, num);
    max_sum = std::max(max_sum, dp);
  }
  return max_sum;
}
