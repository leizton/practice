#ifndef _PRACTICE_COMMON_LOG_H
#define _PRACTICE_COMMON_LOG_H

#include <stdio.h>
#include <errno.h>

#define LOG(fmt, args...) fprintf(stdout, "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##args)
#define LOGERR(fmt, args...) fprintf(stdout, "[%s:%d] errno=%d. " fmt "\n", __FILE__, __LINE__, errno, ##args)

#endif  // _PRACTICE_COMMON_LOG_H