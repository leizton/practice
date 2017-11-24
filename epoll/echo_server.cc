#include "common/header.h"
#include "common/log.h"
#include "common/net/net_util.h"
#include "common/net/epoll_util.h"

const int kPacketMaxSize = 1024;
const int kPacketHeaderSize = 4;

struct Packet {
    char buf[kPacketMaxSize];  // total_size[4 bytes] + user_data[(total_size - 4) bytes]
    int pos = 0;

    int totalSize() {
        return *(int*)buf;
    }

    bool isReadComplete() {
        return pos >= kPacketHeaderSize && pos == totalSize();
    }

    void reset() {
        pos = 0;
    }
}

struct ClientEntry {
    Socket sock;
    int requestCount;
    Packet in;
    Packet out;

    ClientEntry(Socket& _sock) : sock(_sock) {}
}

int main() {
    ServerSocket server_sock = net_util::newServerSocket("127.0.0.1", 8000, 64);
    if (server_sock.inValid()) {
        return -1;
    }

    int epfd = epoll_create(epoll_util::kEpollFdLimit);
    if (epfd < 0) {
        log("epoll_create fail: %d", epfd);
        return -1;
    }

    if (epoll_util::addEvent(epfd, EPOLLIN, server_sock.fd) < 0) {
        log("add listen event fail");
        close(epfd);
        return -1;
    }

    unique_ptr<struct epoll_event[]> events = make_unique<struct epoll_event[]>(epoll_util::kEpollFdLimit);
    unordered_map<int, unique_ptr<ClientEntry>> client_entries;  // map fd to entry

    // accept, io_handle, biz_handle all in main thread
    while (true) {
        int event_num = epoll_wait(epfd, events.get(), epoll_util::kEpollFdLimit, -1);
        for (int event_i = 0; event_i < event_num; ++event_i) {
            struct epoll_event& event = events[event_i];
            if (event.data.fd == server_sock.fd) {
                // accept
                net_util::Socket client_sock = server_sock.accept();
                if (client_sock.inValid()) {
                    continue;
                }

                // add client event
                if (epoll_util::addEvent(epfd, EPOLLIN | EPOLLET, client_sock.fd) < 0) {
                    log("add client event fail. addr=%s:%d", net_util::sockaddrToStr(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    continue;
                }

                // new client entry
                client_entries[client_sock.fd] = make_unique<ClientEntry>(client_sock);
            }
            else if (event.events & EPOLLIN) {
                unique_ptr<ClientEntry>& entry = client_entries[event.data.fd];
                if (entry == nullptr) {
                    log("lost client entry.");
                    close(event.data.fd);
                    continue;
                }

                int n = read(entry->sock.fd, entry->in.buf + entry->in.pos, kPacketMaxSize - entry->in.pos);
                if (n < 0) {
                    log("read fail. addr=%s:%d", net_util::sockaddrToStr(entry->sock.addr.sin_addr), ntohs(entry->sock.addr.sin_port));
                }
                else if (n == 0) {
                    log("disconnect. addr=%s:%d", net_util::sockaddrToStr(entry->sock.addr.sin_addr), ntohs(entry->sock.addr.sin_port));
                    client_entries.erase(entry->sock.fd);
                    close(entry->sock.fd);
                }
                else {
                    // ignore check total_size
                    entry->in.pos += n;
                    if (entry->in.isReadComplete()) {
                        entry->requestCount++;
                    }
                }
            }
            else if (event.events & EPOLLOUT) {
            }
        }
    }

    return 0;
}