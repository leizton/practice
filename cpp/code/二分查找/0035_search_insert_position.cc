#include <util/base.h>

// 右边界
int searchInsertUpperBound(vector<int>& nums, int target) {
  // 等价于 std::upper_bound(nums.begin(), nums.end(), target) - nums.begin()
  int left = 0, right = nums.size(), mid;
  while (left < right) {
    mid = left + ((right - left) >> 1);
    if (target == nums[mid]) {
      left = mid+1;
    } else if (target < nums[mid]) {
      right = mid;
    } else {
      left = mid+1;
    }
  }
  return left;
}

// 左边界
int searchInsert(vector<int>& nums, int target) {
  // 等价于 std::lower_bound(nums.begin(), nums.end(), target) - nums.begin()
  int left = 0, right = nums.size(), mid;
  while (left < right) {
    mid = left + ((right - left) >> 1);
    if (target == nums[mid]) {
      right = mid;
    } else if (target < nums[mid]) {
      right = mid;
    } else {
      left = mid+1;
    }
  }
  return left;
}

int main() {
  vector<int> nums;
  nums = {1, 1, 2, 2, 2, 2, 3};
  assert_T(searchInsertUpperBound(nums, 1) == 2);
  assert_T(searchInsertUpperBound(nums, 2) == 6);
  assert_T(searchInsertUpperBound(nums, 3) == 7);
  assert_T(searchInsert(nums, 1) == 0);
  assert_T(searchInsert(nums, 2) == 2);
  assert_T(searchInsert(nums, 3) == 6);

  nums = {0, 0, 2, 2, 2, 2, 4};
  assert_T(searchInsertUpperBound(nums, 1) == 2);
  assert_T(searchInsertUpperBound(nums, 2) == 6);
  assert_T(searchInsertUpperBound(nums, 3) == 6);
  assert_T(searchInsert(nums, 1) == 2);
  assert_T(searchInsert(nums, 2) == 2);
  assert_T(searchInsert(nums, 3) == 6);
}
