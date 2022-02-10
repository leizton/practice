#include <util/base.h>

int search(vector<int>& nums, int target) {
  int left = 0, right = (int)nums.size(), mid;
  while (left < right) {
    mid = (left + right) >> 1;
    if (target == nums[mid]) {
      return mid;
    } else if (target < nums[mid]) {
      right = mid;
    } else {
      left = mid+1;
    }
  }
  return -1;
}
