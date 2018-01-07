#include <unistd.h>
#include <sys/poll.h>

#include "common/header.h"
#include "common/log.h"

const static int kBufSize = 8;

void readWrapper(const int fd, char* const buf, int* const ridx, const int limit);
void writeWrapper(const int fd, char* const buf, int* const widx, int* const ridx, const int limit);

int main() {
    pollfd fds[2];
    fds[0].fd = STDIN_FILENO,  fds[0].events = POLLIN;
    fds[1].fd = STDOUT_FILENO, fds[1].events = POLLOUT;

    char buf[kBufSize];
    int ridx = 0, widx = 0;

    for (;;) {
        int ret = poll(fds, 2, 1000);
        if (ret < 0) {
            LOGERR("poll fail");
        } else if (ret == 0) {
            LOG("poll timeout");
        } else {
            if ((fds[0].revents & POLLIN)) {
                readWrapper(fds[0].fd, buf, &ridx, kBufSize);
            }
            if (fds[1].revents & POLLOUT) {
                writeWrapper(fds[1].fd, buf, &widx, &ridx, kBufSize);
            }
        }
    }

    return 0;
}

void readWrapper(const int fd, char* const buf, int* const ridx, const int limit) {
    if (*ridx < limit) {
        int rnum = read(fd, buf + *ridx, limit - *ridx);
        if (rnum > 0) {
            *ridx += rnum;
        } else if (rnum < 0) {
            LOGERR("read fail. fd=%d", fd);
        }
    }
}

void writeWrapper(const int fd, char* const buf, int* const widx, int* const ridx, const int limit) {
    int widx_t = *widx;
    const int ridx_t = *ridx;
    if (widx_t < ridx_t) {
        int wnum = write(fd, buf + widx_t, ridx_t - widx_t);
        if (wnum > 0) {
            widx_t += wnum;
            if (widx_t >= ridx_t) {
                *widx = *ridx = 0;
            } else if ((ridx_t - widx_t) >= (limit >> 1)) {
                char* p = buf;
                while (widx_t < ridx_t) {
                    *p++ = buf[widx_t++];
                }
                *widx = 0;
                *ridx = ridx_t - widx_t;
            } else {
                *widx = widx_t;
            }
        } else if (wnum < 0) {
            LOGERR("write fail. fd=%d", fd);
        }
    }
}