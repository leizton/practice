#include <util/base.h>

// 左边界. 返回第一个不小于(即大于等于)target的索引, 可能等于nums.size()
// 等价于 std::lower_bound(nums.begin(), nums.end(), target) - nums.begin()
int searchLower(vector<int>& nums, int target) {
  int l = 0, h = (int)nums.size();
  while (l < h) {
    int mid = (l + h) >> 1;
    if (nums[mid] >= target) {
      h = mid;
    } else {
      l = mid + 1;
    }
  }
  return l;
}

// 右边界. 返回第一个大于target的索引, 可能等于nums.size()
// 等价于 std::upper_bound(nums.begin(), nums.end(), target) - nums.begin()
int searchUpper(vector<int>& nums, int target) {
  int l = 0, h = (int)nums.size();
  while (l < h) {
    int mid = (l + h) >> 1;
    if (nums[mid] > target) {
      h = mid;
    } else {
      l = mid + 1;
    }
  }
  return l;
}

int main() {
  vector<int> nums;
  nums = {1, 1, 2, 2, 2, 2, 3};
  assert_T(searchLower(nums, 1) == 0);
  assert_T(searchLower(nums, 2) == 2);
  assert_T(searchLower(nums, 3) == 6);
  assert_T(searchUpper(nums, 1) == 2);
  assert_T(searchUpper(nums, 2) == 6);
  assert_T(searchUpper(nums, 3) == 7);

  nums = {0, 0, 2, 2, 2, 2, 4};
  assert_T(searchLower(nums, 1) == 2);
  assert_T(searchLower(nums, 2) == 2);
  assert_T(searchLower(nums, 3) == 6);
  assert_T(searchUpper(nums, 1) == 2);
  assert_T(searchUpper(nums, 2) == 6);
  assert_T(searchUpper(nums, 3) == 6);
}
