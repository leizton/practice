/*
epoll_event {
    uint32_t events;
    epoll_data_t data;
}

epoll_data_t union {
    void* ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
}
*/

#ifndef _PRACTICE_COMMON_EPOLL_UTIL_H
#define _PRACTICE_COMMON_EPOLL_UTIL_H

#include <sys/epoll.h>

namespace epoll_util {

static const int kEpollFdLimit = 1024;

int addEvent(int epoll_fd, uint32_t events, int fd);

}  // namespace epoll_util

#endif