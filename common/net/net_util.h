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

ServerSocket newServerSocket(const char* ip, int port, int backlog, bool reuse=false);
Socket newClientSocket(const char* server_ip, uint16_t server_port);

class Socket {
public:
    int fd;
    struct sockaddr_in addr;
private:
    std::shared_ptr<void> ptr_;

public:
    Socket() : fd(-1), ptr_(std::nullptr) {
        memset(&addr, 0, sizeof(addr));
    }

    Socket(const Socket& sock) {
        fd = sock.fd;
        addr = sock.addr;
        ptr = sock.ptr_;
    }

    Socket(const Socket&& sock) {
        Socket(sock);
    }

    Socket& operator=(const Socket& sock) {
        Socket(sock);
    }

    void init(int sock_fd) {
        fd = sock_fd;
        ptr_ = shared_ptr(std::nullptr, [fd, addr](void* p) {
            log("close socket. fd=%d, addr=%s", fd, net_util::sockaddrToStr(addr));
            close(fd);
        });
    }

    bool inValid() const {
        return fd < 0;
    }
};

class ServerSocket : public Socket {
public:
    ClientSocket accept() const;
};

}  // namespace net_util

#endif  // _PRACTICE_COMMON_NETUTIL_H