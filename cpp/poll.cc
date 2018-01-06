#include <sys/poll.h>

#include "common/log.h"

const static int kBufSize = 100;

int main() {
    pollfd fds[2];
    fds[0].fd = STDIN_FILENO,  fds[0].events = POLL_IN;
    fds[1].fd = STDIN_FILEOUT, fds[1].events = POLL_OUT;

    char buf[kBufSize];
    int ridx, widx, rnum, wnum;
    ridx = widx = 0;

    for (;;) {
        int ret = poll(fds, 2, 1000);
        if (ret < 0) {
            LOGERR("poll fail");
        } else if (ret == 0) {
            LOG("poll timeout");
        } else {
            if ((fds[0].revents & POLL_IN) && ridx < kBufSize) {
                rnum = read(fds[0].fd, buf + ridx, kBufSize - ridx);
                if (rnum < 0) {
                    LOGERR("read fail");
                } else if (rnum > 0) {
                    ridx += rnum;
                }
            }
            if (fds[1].revents & POLL_OUT) {
                if (widx < ridx) {
                    wnum = write(fds[1].fd, buf + widx, ridx - widx);
                }
            }
        }
    }

    return 0;
}