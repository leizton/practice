#ifndef _PRACTICE_COMMON_UTIL_H
#define _PRACTICE_COMMON_UTIL_H

#include <string>

namespace util {

inline bool isNullOrEmpty(const char *s) {
    return s == nullptr || *s == '\0';
}

inline bool equal(const char* s1, const char* s2) {
    return (s1 == s2) || (s1 != nullptr && s2 != nullptr && strcmp(s1, s2) == 0);
}

}  // namespace util

#endif