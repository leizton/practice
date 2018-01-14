#ifndef _PRACTICE_COMMON_LOG_H
#define _PRACTICE_COMMON_LOG_H

#include <stdio.h>
#include <errno.h>

#define LOG(fmt, args...) fprintf(stdout, "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##args)
#define LOGERR(fmt, args...) \
    do { int err = errno; fprintf(stdout, "[%s:%d] errno=%d(%s). " fmt "\n", __FILE__, __LINE__, err, strerror(err), ##args); } while(0)

#endif  // _PRACTICE_COMMON_LOG_H