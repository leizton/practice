#include "common/header.h"
#include "common/log.h"
#include "common/net/net_util.h"
#include "common/net/epoll_util.h"

#include "practice_epoll/packet.h"

struct ClientEntry {
    net_util::Socket sock;
    int responseCount;
    Packet in;
    Packet out;

    ClientEntry(net_util::Socket& _sock) : sock(_sock) {}

    void write() {
        if (out.isWriteUnComplete() && out.write(sock.fd) < 0) {
            log("write fail. addr=%s", net_util::sockaddrToStr(sock.addr));
        }
    }
};

int main() {
    net_util::ServerSocket server_sock = net_util::newServerSocket("127.0.0.1", 8000, 100);
    if (server_sock.inValid()) {
        return -1;
    }

    int epfd = epoll_util::createEpoll();
    if (epfd < 0) {
        return -1;
    }

    if (epoll_util::addEvent(epfd, EPOLLIN, server_sock.fd) < 0) {
        log("add listen event fail");
        close(epfd);
        return -1;
    }

    unordered_map<int, unique_ptr<ClientEntry>> client_entries;  // map fd to entry
    auto getClientEntry = [&client_entries](int client_fd) -> unique_ptr<ClientEntry>& {
        unique_ptr<ClientEntry>& entry = client_entries[client_fd];
        if (entry == nullptr) {
            log("lost client entry: %d", client_fd);
            close(client_fd);
        }
        return entry;
    };

    unique_ptr<struct epoll_event[]> events = make_unique<struct epoll_event[]>(epoll_util::kEpollFdLimit);

    // accept, io_handle, biz_handle all in main thread
    while (true) {
        int event_num = epoll_wait(epfd, events.get(), epoll_util::kEpollFdLimit, -1);
        for (int event_i = 0; event_i < event_num; ++event_i) {
            struct epoll_event& event = events[event_i];
            int event_fd = event.data.fd;

            // accept
            if (event_fd == server_sock.fd) {
                net_util::Socket client_sock = server_sock.accept();
                if (client_sock.inValid()) {
                    continue;
                }

                // add client event
                if (epoll_util::addEvent(epfd, EPOLLIN | EPOLLET, client_sock.fd) < 0) {
                    log("add client event fail. addr=%s", net_util::sockaddrToStr(client_addr));
                    continue;
                }

                // new client entry
                client_entries[client_sock.fd] = make_unique<ClientEntry>(client_sock);
                continue;
            }

            unique_ptr<ClientEntry>& entry = getClientEntry(event_fd);
            if (entry == nullptr) {
                continue;
            }

            // io
            if (event.events & EPOLLIN) {
                int read_num = entry->in.read(entry->sock.fd);
                if (read_num < 0) {
                    log("read fail. addr=%s", net_util::sockaddrToStr(entry->sock.addr));
                }
                else if (read_num == 0) {
                    // client断开了连接，server必须关闭socket否则造成socket泄露
                    // netstat查看到的处于CLOSE_WAIT状态的socket就是泄露的
                    log("disconnect. addr=%s", net_util::sockaddrToStr(entry->sock.addr));
                    client_entries.erase(entry->sock.fd);
                }
                else if (entry->in.isReadComplete()) {  // ignore check total_size
                    entry->out.reset();
                    // assert: Packet::HeaderSize + len(response %d) + entry->in.dataSize() < Packet::MaxSize
                    char* const data_out = entry->out.buf + Packet::HeaderSize;
                    int len = snprintf(data_out, Packet::MaxSize, "response %d: ", ++entry->responseCount);
                    memcpy(data_out + len, entry->in.buf + Packet::HeaderSize, entry->in.dataSize());
                    entry->out.limit = Packet::HeaderSize + len + entry->in.dataSize();
                    entry->out.setPacketSize();
                    entry->write();
                }
            }
            else if (event.events & EPOLLOUT) {
                log("[write: %d]", entry->sock.fd);
                entry->write();
            }
        }
    }

    return 0;
}
