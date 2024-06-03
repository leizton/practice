#include <header>

int longestConsecutive(vector<int>& nums) {
  int longest = 0;
  unordered_map<int, bool> num_accessed_map;
  num_accessed_map.reserve(nums.size() << 1);
  for (int e : nums) {
    num_accessed_map[e] = false;
  }
  for (auto& e : num_accessed_map) {
    if (e.second) continue;
    int len = 1;
    for (int i = e.first+1; ; i++) {
      auto iter = num_accessed_map.find(i);
      if (iter == num_accessed_map.end()) break;
      iter->second = true;
      len++;
    }
    for (int i = e.first-1; ; i--) {
      auto iter = num_accessed_map.find(i);
      if (iter == num_accessed_map.end()) break;
      iter->second = true;
      len++;
    }
    longest = max(longest, len);
  }
  return longest;
}

int longestConsecutiveSort(vector<int>& nums) {
  if (nums.size() <= 1u) return (int)nums.size();
  sort(nums.begin(), nums.end());
  int longest = 1;  // 最大长度
  int accum = 1;    // 累计长度
  for (int i = 1; i < (int)nums.size(); i++) {
    if (nums[i] == nums[i-1]+1) {
      accum++;
      longest = max(longest, accum);
    } else if (nums[i] != nums[i-1]) {
      accum = 1;
    }
  }
  return longest;
}