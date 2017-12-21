#ifndef _PRACTICE_COMMON_LOG_H
#define _PRACTICE_COMMON_LOG_H

#include <stdio.h>

#define LOG(fmt, args...) fprintf(stdout, "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##args)

#endif  // _PRACTICE_COMMON_LOG_H