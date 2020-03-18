#pragma once

#include "util/cpp_lib.h"
#include "util/print.h"
#include "util/utils.h"
#include "util/too.h"
#include "util/unit_test.h"


/**
 * time
 */
inline uint64_t nowSec() {
  return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline uint64_t nowMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline uint64_t nowUs() {
  return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline std::chrono::duration<int, std::ratio<1LL, 1LL>> buildDurationSec(int sec) {
  return std::chrono::duration<int, std::ratio<1LL, 1LL>>(sec);
}
inline std::chrono::duration<int, std::milli> buildDurationMs(int ms) {
  return std::chrono::duration<int, std::milli>(ms);
}
inline std::chrono::duration<int, std::micro> buildDurationUs(int us) {
  return std::chrono::duration<int, std::micro>(us);
}
inline void sleepSec(int sec) {
  std::this_thread::sleep_for(buildDurationSec(sec));
}
inline void sleepMs(int ms) {
  std::this_thread::sleep_for(buildDurationMs(ms));
}
inline void sleepUs(int us) {
  std::this_thread::sleep_for(buildDurationUs(us));
}
