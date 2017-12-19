#ifndef _PRACTICE_COMMON_LOG_H
#define _PRACTICE_COMMON_LOG_H

#include <stdio.h>

#define log(fmt, args...) //fprintf(stdout, "[%s:%d] " fmt "\n", __FILE__, __LINE__, args)
#define logs(s) //fprintf(stdout, "[%s:%d] " s "\n", __FILE__, __LINE__)

#endif  // _PRACTICE_COMMON_LOG_H