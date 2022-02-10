#include <util/base.h>

static int bad_min = 1;
bool isBadVersion(int bad) {
  return bad >= bad_min;
}

// 数组[1..n], 从某个数开始 isBadVersion() 返回true
// 即 for 1<=i<bad, isBadVersion(i) = false; for bad<=i<=n, isBadVersion(i) = true
// 返回 bad
// 二分法
int firstBadVersion(int n) {
  if (isBadVersion(1)) return 1;
  if (!isBadVersion(n)) return n;
  unsigned int left = 1, right = (unsigned int)n+1, mid; // 用 unsigned 防止 (n+1) 越界
  while (left < right) {
    mid = (left + right) >> 1;
    if (isBadVersion(int(mid))) {
      if (isBadVersion(int(mid-1))) {
        right = mid;
      } else {
        return mid;
      }
    } else {
      left = mid+1;
    }
  }
  return left;
}

int main() {
  bad_min = 3;
  assert_T(firstBadVersion(17) == bad_min);
  bad_min = 7;
  assert_T(firstBadVersion(17) == bad_min);
  bad_min = 13;
  assert_T(firstBadVersion(17) == bad_min);
  bad_min = 17;
  assert_T(firstBadVersion(17) == bad_min);
  bad_min = INT32_MAX - 1;
  assert_T(firstBadVersion(INT32_MAX) == bad_min);
  bad_min = INT32_MAX;
  assert_T(firstBadVersion(INT32_MAX) == bad_min);
}
