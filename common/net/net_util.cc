#include <string>
#include <thread>

#include <unistd.h>

#include "common/net/net_util.h"
#include "common/log.h"
#include "common/util.h"

namespace net_util {

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
    if (util::isNullOrEmpty(ip) || util::equal("0", ip) ||
        util::equal("*", ip) || util::equal("0.0.0.0", ip)) {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else {
        addr.sin_addr.s_addr = inet_addr(ip);
    }
}

thread_local char gtlIpAddr[20];
char* sockaddrToStr(struct sockaddr_in& addr) {
    inet_ntop(AF_INET, (void*) &addr.sin_addr, gtlIpAddr, sizeof(gtlIpAddr));
    return gtlIpAddr;
}

ServerSocket newServerSocket(const char* ip, uint16_t port, int backlog, bool reuse=false) {
    if (port == 0) {
        log("port 0 is reserved");
        return -1;
    }

    // create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // domain, socket_type, protocol
    if (server_fd < 0) {
        log("open new socket fail: %d", server_fd);
        return ServerSocket();
    }
    if (setNonBlock(server_fd) < 0) {
        log("setNonBlock fail");
        return ServerSocket();
    }
    if (reuse) {
        int nReuseAddr = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &nReuseAddr, sizeof(nReuseAddr)) < 0) {
            log("setsockopt fail");
            return ServerSocket();
        }
    }
    ServerSocket server_sock;
    server_sock.init(server_fd);

    // bind
    setSocketAddr(ip, port, server_sock.addr);
    if (bind(server_fd, (sockaddr*)&server_sock.addr, sizeof(server_sock.addr)) < 0) {
        log("bind socket fail");
        return ServerSocket();
    }

    // listen
    if (listen(server_fd, backlog) < 0) {
        log("listen socket fail");
        return ServerSocket();
    }

    log("create server socket. fd=%d, addr=%s:%d", server_fd, ip, port);
    return server_sock;
}

Socket ServerSocket::accept() {
    Socket client_sock;
    int client_fd = accept(server_sock.fd, (sockaddr*)&client_sock.addr, sizeof(client_sock.addr));
    if (client_fd < 0) {
        log("accept fail");
        return Socket();
    }
    client_sock.init(client_fd);

    if (net_util::setNonBlock(client_fd) < 0) {
        log("set non-block fail. addr=%s:%d", net_util::sockaddrToStr(client_addr.sin_addr), ntohs(client_addr.sin_port));
        return Socket();
    }

    log("new connection. addr=%s:%d", net_util::sockaddrToStr(client_addr.sin_addr), ntohs(client_addr.sin_port));
    return client_sock;
}

}  // namespace net_util