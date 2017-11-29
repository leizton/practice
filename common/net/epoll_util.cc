#include <sys/epoll.h>

#include "common/log.h"

namespace epoll_util {

int createEpoll() {
    int epfd = epoll_create(kEpollFdLimit);
    if (epfd < 0) {
        logs("epoll_create fail");
        return -1;
    }
    return epfd;
}

int addEvent(int epoll_fd, uint32_t events, int fd) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

}