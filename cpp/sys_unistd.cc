#include <unistd.h>
#include <sys/time.h>

#include "common/log.h"

void _timeval() {
    timeval tv = { 60, 1 };
    LOG("time: %ld.%06lds", tv.tv_sec, tv.tv_usec);

    timeval start, end;
    gettimeofday(&start, nullptr);
    usleep(500);
    gettimeofday(&end, nullptr);
    LOG("sleep 500us, time diff: %ld.%06lds",
        end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
}

int main() {
    _timeval();

    return 0;
}