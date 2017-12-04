#ifndef _PRACTICE_COMMON_UTIL_H
#define _PRACTICE_COMMON_UTIL_H

#include <cstring>

namespace util {

inline bool isNullOrEmpty(const char *s) {
    return s == nullptr || *s == '\0';
}

inline bool equal(const char* s1, const char* s2) {
    return (s1 == s2) || (s1 != nullptr && s2 != nullptr && strcmp(s1, s2) == 0);
}

int strlen(char* s, int max) {
    if (s == nullptr) {
        return -1;
    }
    char* p = s + (max - 1);
    char old = *p;
    *p = '\0';
    int len = ::strlen(s);
    *p = old;
    return len;
}

}  // namespace util

#endif