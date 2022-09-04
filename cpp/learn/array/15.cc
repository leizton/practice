#include <util/base.h>

/*
给你一个整数数组 nums ，判断是否存在三元组 [nums[i], nums[j], nums[k]] 满足 i != j、i != k 且 j != k ，同时还满足 nums[i] + nums[j] + nums[k] == 0 。
请你返回所有和为 0 且不重复的三元组。
注意：答案中不可以包含重复的三元组。
*/

vector<vector<int>> threeSum(vector<int>& nums) {
  const int n = nums.size();
  sort(nums.begin(), nums.end());
  //
  vector<vector<int>> ans;
  for (int i = 0; i < n-2; i++) {
    if (nums[i] > 0) break;
    if (i > 0 && nums[i] == nums[i-1]) continue;
    int target = -nums[i];
    int p = i+1, q = n-1;
    while (p < q) {
      int v = nums[p] + nums[q];
      if (v < target) {
        p++;
      } else if (v > target) {
        q--;
      } else {
        ans.push_back({nums[i], nums[p++], nums[q--]});
        while (p < q && nums[p] == nums[p-1]) p++;
        while (p < q && nums[q] == nums[q+1]) q--;
      }
    }
  }
  return ans;
}

int main() {
  auto test = [] (vector<int> nums) {
    return threeSum(nums);
  };
  print(test({-1,0,1,2,-1,-4}));
  print(test({-2,0,0,2,2}));
}
