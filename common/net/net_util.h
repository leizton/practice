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

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace net_util {

int newTcpSocket();
int setNonBlock(int sock_fd);

void setSocketAddr(const char* ip, uint16_t port, struct sockaddr_in& addr);
char* sockaddrToStr(struct sockaddr_in& addr);

int newServerSocket(const char* ip, int port, int backlog, bool reuse=false);

}

#endif  // _PRACTICE_COMMON_NETUTIL_H