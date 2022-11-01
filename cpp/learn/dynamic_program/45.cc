#include <header>

/*
给你一个非负整数数组 nums ，你最初位于数组的第一个位置。
数组中的每个元素代表你在该位置可以跳跃的最大长度。
你的目标是使用最少的跳跃次数到达数组的最后一个位置。
假设你总是可以到达数组的最后一个位置
nums的元素可能取值0

输入: nums = [2,3,1,1,4]
输出: 2
解释: 跳到最后一个位置的最小跳跃数是 2。
     从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。
*/

int jump_v0(vector<int>& nums) {
  const int n = nums.size();
  if (n <= 1) return 0;

  // dp[i]: 到达位置i的最小跳跃数
  vector<int> dp(n, INT32_MAX);
  dp[0] = 0;

  for (int i = 0; i < n; i++) {
    int jump_num = dp[i] + 1;
    int reach = std::min(i+nums[i], n-1);
    for (int j = i+1; j <= reach; j++) {
      dp[j] = std::min(dp[j], jump_num);
    }
  }
  return dp[n-1];
}

// 利用dp序列的单调性
int jump_v1(vector<int>& nums) {
  const int n = nums.size();
  if (n <= 1) return 0;

  // dp[i]: 到达位置i的最小跳跃数
  vector<int> dp(n, -1);
  dp[0] = 0;
  int reach = std::min(nums[0], n-1);
  for (int i = 1; i <= reach; i++) {
    dp[i] = 1;
  }
  int reach_max = reach;

  for (int i = 1; i < n; i++) {
    reach = std::min(i+nums[i], n-1);
    if (reach > reach_max) {
      // [reach_max+1, reach]是没有到过的地方, 通过i可以到达
      for (int j = reach_max+1; j <= reach; j++) {
        // 因为dp是单调不减序列. 所以 ∀ i<k<j 有 dp[k]>=dp[i]
        // 所以从i跳到j最优
        dp[j] = dp[i] + 1;
      }
      reach_max = reach;
    }
  }
  return dp[n-1];
}

// 改成空间O(1)
int jump(vector<int>& nums) {
  const int n = nums.size();
  if (n <= 1) return 0;
  int curr_range_begin = 0, curr_range_reach = 0;
  int step_num = 0;
  while (curr_range_reach < n-1) {
    int next_range_reach = 0;
    for (int i = curr_range_begin; i <= curr_range_reach; i++) {
      next_range_reach = std::max(next_range_reach, i + nums[i]);
    }
    curr_range_begin = curr_range_reach + 1;
    curr_range_reach = next_range_reach;
    step_num++;
  }
  return step_num;
}
