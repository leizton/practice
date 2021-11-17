#include <util/base.h>

int main() {
  static const uint64_t one = 1u;
  static const int test_num = 100000;

  assert_eq(-1, getFirstNonZeroBitPos_v1(0));
  for (uint64_t i = 0; i < 64; i++) {
    uint64_t x = one << i;
    assert_eq((int)i, getFirstNonZeroBitPos_v1(x));
  }

  assert_eq(-1, getFirstNonZeroBitPos(0));
  for (uint64_t i = 0; i < 64; i++) {
    uint64_t x = one << i;
    assert_eq((int)i, getFirstNonZeroBitPos(x));
  }

  auto t1 = nowUs();
  int r1 = 0;
  for (int i = 0; i < test_num; i++) {
    for (uint64_t j = 0; j < 64; j++) {
      uint64_t x = one << i;
      r1 += getFirstNonZeroBitPos(x);
    }
  }
  t1 = nowUs() - t1;

  auto t2 = nowUs();
  int r2 = 0;
  for (int i = 0; i < test_num; i++) {
    for (uint64_t j = 0; j < 64; j++) {
      uint64_t x = one << i;
      r2 += getFirstNonZeroBitPos_v1(x);
    }
  }
  t2 = nowUs() - t2;

  assert_eq(r1, r2);
  print(t1, t2);
}
