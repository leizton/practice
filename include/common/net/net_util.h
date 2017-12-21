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

#include <cstring>
#include <memory>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "common/log.h"

namespace net_util {

class Socket;
class ServerSocket;

int setNonBlock(int sock_fd);

void setSocketAddr(const char* ip, uint16_t port, struct sockaddr_in& addr);
char* sockaddrToStr(struct sockaddr_in& addr);

ServerSocket newServerSocket(const char* ip, uint16_t port, int backlog, bool reuse);
Socket newClientSocket(const char* server_ip, uint16_t server_port);

class Socket {
public:
    int fd;
    struct sockaddr_in addr;
private:
    std::shared_ptr<void> ptr_;

public:
    Socket() : fd(-1), ptr_(nullptr) {
        memset(&addr, 0, sizeof(addr));
    }

    Socket(const Socket& sock) {
        fd = sock.fd;
        addr = sock.addr;
        ptr_ = sock.ptr_;
    }

    Socket(const Socket&& sock) {
        fd = sock.fd;
        addr = sock.addr;
        ptr_ = sock.ptr_;
    }

    Socket& operator=(const Socket& sock) {
        fd = sock.fd;
        addr = sock.addr;
        ptr_ = sock.ptr_;
        return *this;
    }

    void init(int sock_fd) {
        fd = sock_fd;
        struct sockaddr_in addr = this->addr;
        ptr_ = std::shared_ptr<void>(nullptr, [sock_fd, addr](void* p) {
            log("close socket. fd=%d, addr=%s", sock_fd, net_util::sockaddrToStr(addr));
            if (::close(sock_fd) < 0) {
                log("close socket fail. fd=%d, addr=%s", sock_fd, net_util::sockaddrToStr(addr));
            }
        });
    }

    bool inValid() const {
        return fd < 0;
    }
};

class ServerSocket : public Socket {
public:
    Socket accept() const;
};

}  // namespace net_util

#endif  // _PRACTICE_COMMON_NETUTIL_H