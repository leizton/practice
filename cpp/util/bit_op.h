#pragma once

inline int getFirstNonZeroBitPos(uint64_t x) {
  return 63 - __builtin_clzll(x);
}

inline int getFirstNonZeroBitPos_v1(uint64_t x) {
  static const int tb[67] = {
      -1, 0, 1, 39, 2, 15, 40, 23, 3, 12, 16, 59, 41, 19, 24, 54,
      4, 0, 13, 10, 17, 62, 60, 28, 42, 30, 20, 51, 25, 44, 55, 47,
      5, 32, 0, 38, 14, 22, 11, 58, 18, 53, 63, 9, 61, 27, 29, 50,
      43, 46, 31, 37, 21, 57, 52, 8, 26, 49, 45, 36, 56, 7, 48, 35,
      6, 34, 33};
  x = x & ((x - 1) ^ UINT64_MAX);
  return tb[(x % 67)];
}

inline int getFirstNonZeroBitPos_v2(uint64_t v) {
  static const int MultiplyDeBruijnBitPosition[64] = {
      63, 0, 58, 1, 59, 47, 53, 2,
      60, 39, 48, 27, 54, 33, 42, 3,
      61, 51, 37, 40, 49, 18, 28, 20,
      55, 30, 34, 11, 43, 14, 22, 4,
      62, 57, 46, 52, 38, 26, 32, 41,
      50, 36, 17, 19, 29, 10, 13, 21,
      56, 45, 25, 31, 35, 16, 9, 12,
      44, 24, 15, 8, 23, 7, 6, 5};
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  v -= v >> 1;
  return MultiplyDeBruijnBitPosition[(v * 0x07EDD5E59A4E28C2ULL) >> 58];
}

inline int getFirstNonZeroBitPos_v3(uint64_t x) {
  static const uint64_t flag1 = 0xFFFFFFFF;
  static const uint64_t flag2 = 0xFFFF;
  static const uint64_t flag3 = 0xFF;
  static const uint64_t flag4 = 0x0F;
  static const uint64_t flag5 = 0x03;
  static const uint64_t flag6 = 0x01;
  x = x & ((x - 1) ^ UINT64_MAX);
  if (x == 0u) {
    return -1;
  }
  int pos = 0;
  if (!(x & flag1)) {
    pos |= 32;
    x >>= 32;
  }
  if (!(x & flag2)) {
    pos |= 16;
    x >>= 16;
  }
  if (!(x & flag3)) {
    pos |= 8;
    x >>= 8;
  }
  if (!(x & flag4)) {
    pos |= 4;
    x >>= 4;
  }
  if (!(x & flag5)) {
    pos |= 2;
    x >>= 2;
  }
  if (!(x & flag6)) {
    pos |= 1;
  }
  return pos;
}
