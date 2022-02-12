#include <util/base.h>

vector<int> sortedSquares(vector<int>& nums) {
  vector<int> squares(nums.size(), 0);
  int left = 0, right = (int)nums.size()-1;
  int i = right;
  while (left <= right) {
    int lv = (nums[left] * nums[left]);
    int rv = (nums[right] * nums[right]);
    if (lv <= rv) {
      squares[i--] = rv;
      right--;
    } else {
      squares[i--] = lv;
      left++;
    }
  }
  return squares;
}
