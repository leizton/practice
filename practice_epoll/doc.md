```c
#include <sys/epoll.c>

// return: epfd
int  epoll_create(int size);

union epoll_data_t {
    void*     ptr;
    int       fd;
    uint32_t  u32;
    uint64_t  u64;
};

// events: EPOLLIN/EPOLLOUT  EPOLLET
struct epoll_event {
    uint32_t      events;
    epoll_data_t  data;
};

// op: EPOLL_CTL_ADD/MOD/DEL
int  epoll_ctl(int epfd, int op, int fd, epoll_event* events);

// timeout: -1,block indefinitely; 0,return immediately; >0,milliseconds
int  epoll_wait(int epfd, epoll_event* events, int maxevents, int timeout);
```