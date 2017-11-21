#include <sys/epoll.h>

namespace epoll_util {

int addEvent(int epoll_fd, uint32_t events, int fd) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

}