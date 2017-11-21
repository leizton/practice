#include <string>

#include "common/net/net_util.h"
#include "common/log.h"
#include "common/wh_util.h"

namespace net_util {

int newTcpSocket() {
    // open socket
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // domain, socket_type, protocol
}

int setNonBlock(int sock_fd) {
    /**
     * 在读不到数据或写缓冲满时, 会立即return, 而不被阻塞
     * 设置O_NDELAY使读不到数据时立即返回0, 然而读到fd末尾也是返回0
     * 设置O_NONBLOCK使读不到数据时立即返回-1, 读到末尾返回0
     */
    int flags = fcntl(sock_fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    flags |= O_NDELAY;
    return fcntl(sock_fd, F_SETFL, flags);
}

void setSocketAddr(const char* ip, uint16_t port, struct sockaddr_in& addr) {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (wh_util::isNullOrEmpty(ip) || wh_util::equal("0", ip)
        || wh_util::equal("*", ip) || wh_util::equal("0.0.0.0", ip)) {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else {
        addr.sin_addr.s_addr = inet_addr(ip);
    }
}

char gIpAddr[20];
char* sockaddrToStr(struct sockaddr_in& addr) {
    inet_ntop(AF_INET, (void*) &addr.sin_addr, gIpAddr, sizeof(gIpAddr));
    return gIPAddr;
}

int newServerSocket(const char* ip, uint16_t port, int backlog, bool reuse=false) {
    if (port == 0) {
        log("port 0 is reserved");
        return -1;
    }

    int sock_fd = newTcpSocket();
    if (sock_fd < 0) {
        log("open new socket fail: %d", sock_fd);
        return -1;
    }
    if (setNonBlock(sock_fd) < 0) {
        log("setNonBlock fail");
        close(sock_fd);
        return -1;
    }
    if (reuse) {
        int nReuseAddr = 1;
        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &nReuseAddr, sizeof(nReuseAddr)) < 0) {
            log("setsockopt fail");
        }
    }

    // bind
    struct sockaddr_in addr;
    setSocketAddr(ip, port, addr);
    if (bind(sock_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        log("bind socket fail");
        close(sock_fd);
        return -1;
    }

    // listen
    if (listen(sock_fd, backlog) < 0) {
        log("listen socket fail");
        close(sock_fd);
        return -1;
    }

    log("create server socket. fd=%d, addr=%s:%d", sock_fd, ip, port);
    return sock_fd;
}

}  // namespace net_util