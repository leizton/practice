#include <util/base.h>

void rotate(vector<int>& nums, int k) {
  k = k % ((int)nums.size());
  if (k == 0) return;
  static auto reverse = [](vector<int>& nums, int left, int right) {
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

void rotate(vector<int>& nums, int k) {
  static auto gcd = [](int n, int k) {
    if (n < k) std::swap(n, k);
    if (k == 0) return 0;
    for (int t; k != 0; ) {
      t = n;
      n = k;
      k = t % k;
    }
    return n;
  };
  int n = (int)nums.size();
  k = k % n;
  if (k == 0) return;
  int G = gcd(n, k);
  int b = n / G;
  for (int i = 0; i < G; i++) {
    int prev = nums[i];
    int curr = i + k;
    for (int j = 1; j < b; j++) {
      int tmp = nums[curr % n];
      nums[curr % n] = prev;
      prev = tmp;
      curr += k;
    }
    nums[i] = prev;
  }
}
