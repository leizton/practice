#include <util/base.h>

/*
给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出和为目标值target的那两个整数，并返回它们的数组下标。
你可以假设每种输入有且只有一个答案。但是，数组中同一个元素在答案里不能重复出现。
你可以按任意顺序返回答案。
*/

vector<int> twoSum(vector<int>& nums, int target) {
  unordered_map<int, int> m;
  for (int i = 0; i < nums.size(); i++) {
    auto it = m.find(target - nums[i]);
    if (it != m.end()) {
      return vector<int>{it->second, i};
    }
    m[nums[i]] = i;
  }
  return vector<int>();
}
