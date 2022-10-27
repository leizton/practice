#include <header>

/*
198.cc

你是一个专业的小偷，计划偷窃沿街的房屋。
每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统
如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警

输入：[1,2,3,1]
输出：4
解释：偷窃 1 号房屋 (金额 = 1) ，然后偷窃 3 号房屋 (金额 = 3)。
     偷窃到的最高金额 = 1 + 3 = 4 。
*/
int rob_198(vector<int>& nums, int begin, int end) {
  const int n = end - begin;
  if (n == 0) return 0;
  if (n == 1) return nums[begin];
  if (n == 2) return std::max(nums[begin], nums[begin+1]);

  // dp[i]: 截止到i且偷i的最大收益. 空间复杂度可以改成O(1)
  vector<int> dp(n);
  dp[0] = nums[0];
  dp[1] = nums[1];
  dp[2] = nums[2] + dp[0];

  int max_profit = std::max({dp[0], dp[1], dp[2]});
  for (int i = begin+3; i < end; i++) {
    dp[i] = nums[i] + std::max(dp[i-2], dp[i-3]);
    max_profit = std::max(max_profit, dp[i]);
    /*
      设 ndp[i] 表示截止到i且不偷i的最大收益
      ndp[i] = max(dp[i-1], ndp[i-1])
      ndp[i-1] = max(dp[i-2], ndp[i-2])
      ndp[i-2] = max(dp[i-3], ndp[i-3])
      ...
      ndp[2] = max(dp[1], ndp[1])
      ndp[1] = dp[0]
      所以 ndp[i] = max(dp[i-1], dp[i-2], dp[i-3], ..., dp[0])
    */
  }
  return max_profit;
}

/*
213.cc

所有的房屋都 围成一圈 ，这意味着第一个房屋和最后一个房屋是紧挨着的

输入：nums = [2,3,2]
输出：3
解释：你不能先偷窃 1 号房屋（金额 = 2），然后偷窃 3 号房屋（金额 = 2）, 因为他们是相邻的。
*/
int rob_213(vector<int>& nums) {
  int n = (int)nums.size();
  if (n < 4) {
    int max_profit = 0;
    for (int v : nums) max_profit = std::max(max_profit, v);
    return max_profit;
  }

  // 如果选择nums[0], nums[1]和nums[n-1]不能选, nums[2:n-2]自由选, 所以考虑nums[2:n-2]的最大收益
  int max_profit = nums[0] + rob_198(nums, 2, n-1);

  // 如果不选择nums[0], nums[1:n-1]自由选
  max_profit = std::max(max_profit, rob_198(nums, 1, n));

  return max_profit;
}