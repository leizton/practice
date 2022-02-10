#include <util/base.h>

// hash
vector<int> twoSum_hash(vector<int>& nums, int target) {
  unordered_map<int, int> index;
  for (int i = 0; i < (int)nums.size(); i++) {
    auto iter = index.find(target - nums[i]);
    if (iter != index.end()) {
      return {iter->second, i};
    }
    index[nums[i]] = i;
  }
  return {0, 0};
}

// 双指针
vector<int> twoSum(vector<int>& nums, int target) {
  vector<pair<int,int>> num2idx;
  num2idx.reserve(nums.size());
  for (int i = 0; i < (int)nums.size(); i++) {
    num2idx.emplace_back(nums[i], i);
  }
  std::sort(num2idx.begin(), num2idx.end());
  int p = 0, q = (int)nums.size() - 1;
  while (p < q) {
    int sum = num2idx[p].first + num2idx[q].first;
    if (sum < target) {
      p++;
    } else if (sum > target) {
      q--;
    } else {
      return {num2idx[p].second, num2idx[q].second};
    }
  }
  return {0, 0};
}

int main() {
  vector<int> nums;
  nums = {2,7,11,15};
  print(twoSum(nums, 9));
}