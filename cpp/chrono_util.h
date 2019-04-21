#include <chrono>

namespace ChronoUtil {

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

inline TimePoint now() {
  return std::chrono::system_clock::now();
}

inline uint64_t timeDiff(const TimePoint& end, const TimePoint& start) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

}
