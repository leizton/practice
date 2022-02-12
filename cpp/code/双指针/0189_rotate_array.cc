#include <util/base.h>

void rotate(vector<int>& nums, int k) {
  k = k % ((int)nums.size());
  if (k == 0) return;
  auto reverse = [](vector<int>& nums, int left, int right) {
    while (left < right) {
      int tmp = nums[left];
      nums[left] = nums[right];
      nums[right] = tmp;
      left++;
      right--;
    }
  };
  reverse(nums, 0, (int)nums.size()-1);
  reverse(nums, 0, k-1);
  reverse(nums, k, (int)nums.size()-1);
}
