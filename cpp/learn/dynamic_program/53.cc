#include <util/base.h>

/*
给你一个整数数组 nums ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和
和 42.cc 类似
*/

int maxSubArray(vector<int>& nums) {
  // dp 是以(i-1)作终点的连续子数组的最大值
  int ans = INT32_MIN, dp = 0;
  for (int num : nums) {
    dp = std::max(0, dp) + num;
    ans = std::max(ans, dp);
  }
  return ans;
}
