/*
sockaddr_in {
    short    sin_family;
    ushort   sin_port;
    in_addr  sin_addr;
    char     sin_zero[8];
}

in_addr {
    ulong s_addr;
}
*/

#ifndef _PRACTICE_COMMON_NETUTIL_H
#define _PRACTICE_COMMON_NETUTIL_H

#include <memory>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "common/log.h"

namespace net_util {

int setNonBlock(int sock_fd);

void setSocketAddr(const char* ip, uint16_t port, struct sockaddr_in& addr);
char* sockaddrToStr(struct sockaddr_in& addr);

int newServerSocket(const char* ip, int port, int backlog, bool reuse=false);

class ServerSocket {
public:
    int fd;
private:
    std::shared_ptr<void> ptr_;

public:
    ServerSocket() :fd(-1), ptr_(std::nullptr) {}

    ServerSocket(int sock_fd)
        : fd(sock_fd),
          ptr_(std::nullptr, [fd](void* p) { log("close ServerSocket: %d", fd); close(fd); }) {}
};

}  // namespace net_util

#endif  // _PRACTICE_COMMON_NETUTIL_H