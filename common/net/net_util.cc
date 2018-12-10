#include <string>
#include <thread>

#include <unistd.h>
#include <fcntl.h>

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

int setNonBlockWrapper(int sock_fd) {
    if (setNonBlock(sock_fd) != 0) {
        LOGERR("setNonBlock fail. fd=%d", sock_fd);
        if (::close(sock_fd) != 0) {
            LOGERR("close fail. fd=%d", sock_fd);
        }
        return -1;
    }
    return 0;
}

void setSocketAddr(const char* ip, uint16_t port, sockaddr_in& addr) {
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

static const int kIpAddrMaxLen = 24;
thread_local char gtlIpAddr[kIpAddrMaxLen];

char* sockaddrToStr(const sockaddr_in& addr) {
    inet_ntop(AF_INET, (void*) &addr.sin_addr, gtlIpAddr, kIpAddrMaxLen);
    int idx = util::strlen(gtlIpAddr, kIpAddrMaxLen);
    snprintf(gtlIpAddr + idx, kIpAddrMaxLen - idx, ":%d", ntohs(addr.sin_port));
    return gtlIpAddr;
}

int newTcpSocket() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // domain, socket_type, protocol
    if (sock_fd < 0) {
        LOGERR("open new socket fail");
        return -1;
    }
    return sock_fd;
}

ServerSocket newServerSocket(const char* ip, uint16_t port, int backlog, bool reuse) {
    ServerSocket server_sock;
    if (port == 0) {
        LOG("port 0 is reserved");
        return ServerSocket();
    }

    // create socket
    int server_fd = newTcpSocket();
    if (server_fd < 0) {
        return ServerSocket();
    }

    // reuse
    if (reuse) {
        int reuse_opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_opt, sizeof(reuse_opt)) != 0) {
            LOGERR("setsockopt fail");
            return ServerSocket();
        }
    }

    // bind
    setSocketAddr(ip, port, server_sock.addr);
    if (bind(server_fd, (sockaddr*)&server_sock.addr, sizeof(server_sock.addr)) != 0) {
        LOG("bind socket fail");
        return ServerSocket();
    }

    // listen
    if (listen(server_fd, backlog) != 0) {
        LOGERR("listen socket fail");
        return ServerSocket();
    }

    // set non block
    if (setNonBlockWrapper(server_fd) != 0) {
        return ServerSocket();
    }

    server_sock.init(server_fd);
    LOG("create server socket. fd=%d, addr=%s:%d", server_fd, ip, port);
    return server_sock;
}

Socket newClientSocket(const char* server_ip, uint16_t server_port) {
    Socket client_sock;

    // create socket
    int client_fd = newTcpSocket();
    if (client_fd < 0) {
        return Socket();
    }

    // connect
    setSocketAddr(server_ip, server_port, client_sock.addr);
    if (connect(client_fd, (sockaddr*)&client_sock.addr, sizeof(client_sock.addr)) != 0) {
        LOGERR("connect %s fail", net_util::sockaddrToStr(client_sock.addr));
        return Socket();
    }

    // set non block
    if (setNonBlockWrapper(client_fd) != 0) {
        return Socket();
    }

    client_sock.init(client_fd);
    LOG("connected %s", net_util::sockaddrToStr(client_sock.addr));
    return client_sock;
}

Socket ServerSocket::accept() const {
    Socket client_sock;
    socklen_t addr_len = 0;
    int client_fd = ::accept(this->fd, (sockaddr*)&client_sock.addr, &addr_len);
    if (client_fd < 0) {
        LOGERR("accept fail");
        return Socket();
    }
    client_sock.init(client_fd);

    if (net_util::setNonBlock(client_fd) != 0) {
        LOGERR("set non-block fail. addr=%s", net_util::sockaddrToStr(client_sock.addr));
        return Socket();
    }

    LOG("new connection. addr=%s", net_util::sockaddrToStr(client_sock.addr));
    return client_sock;
}

}  // namespace net_util
