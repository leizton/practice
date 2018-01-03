#include <sys/time.h>

#include "common/log.h"

int main() {
    timeval tv = { 60, 1 };
    LOG("time: %ld.%06lds", tv.tv_sec, tv.tv_usec);

    return 0;
}