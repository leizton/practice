#include "common/header.h"
#include "common/log.h"
#include "common/net/net_util.h"
#include "common/net/epoll_util.h"

#include "epoll/include/packet.h"

struct Connect {
    static atomic<int> ObjCount(0);
    int id;
    Socket sock;
    Packet in;
    Packet out;
    string data;
    int maxRequestCount;
    int requestCount;

    Connect(Socket _sock, const string& _data, const int _maxRequestCount)
        : id(++ObjCount), sock(_sock), data(_data), maxRequestCount(_maxRequestCount) {}

    ~Connect() {
        --ObjCount;
    }

    bool request();
    void encode();
    void write();
    bool read();
};

bool Connect::request() {
    if (++requestCount > maxRequestCount) {
        return false;
    }
    encode();
    write();
    return true;
}

void Connect::encode() {
    out.reset();
    char* const data_out = out.buf + Packet::HeaderSize;
    int len = snprintf(data_out, Packet::MaxSize, "request %d: ", ++requestCount);
    memcpy(data_out + len, data.c_str(), data.length());
    out.limit = Packet::HeaderSize + len + data.length();
    out.setPacketSize();
}

void Connect::write() {
    if (out.isWriteUnComplete() && out.write(sock.fd) < 0) {
        log("write error. connectId=%d", id);
    }
}

bool read() {
    int read_num = in.read(sock.fd);
    if (in.isReadComplete()) {
        string response(in.buf + Packet::HeaderSize, in.dataSize());
        log("%s", response.c_str());
        return true;
    }
    return false;
}

int main() {
    string datas[] = { "hello, world", "hey, world", "hi, world" };

    int epfd = epoll_util::createEpoll();
    if (epfd < 0) {
        return -1;
    }

    unordered_map<int, unique_ptr<Connect>> connects;
    for (auto& data : datas) {
        net_util::Socket client_sock = net_util::newClientSocket("127.0.0.1", 8000);
        if (client_sock.invalid()) {
            return -1;
        }

        if (epoll_util::addEvent(epfd, EPOLLIN | EPOLLET, client_sock.fd) < 0) {
            log("add client event fail");
            close(epfd);
            return -1;
        }
        connects[client_sock.fd] = make_unique<Connect>(client_sock, data, connects.size() + 1);
    }
    for (auto it = connects.begin(); it != connects.end() ) {
        if (it->second->request()) {
            ++it;
        }
        else {
            connects.erase(it++);
        }
    }

    unique_ptr<struct epoll_event[]> events = make_unique<struct epoll_event[]>(epoll_util::kEpollFdLimit);

    while (!connects.empty()) {
        int event_num = epoll_wait(epfd, events.get(), epoll_util::kEpollFdLimit, -1);
        for (int event_i = 0; event_i < event_num; ++event_i) {
            struct epoll_event& event = events[event_i];
            unique_ptr<Connect>& connect = connects[event.data.fd];
            if (connect == nullptr) {
                log("lost connect: %d", event.data.fd);
                close(event.data.fd);
                continue;
            }

            if (event.events & EPOLLIN) {
                log("[read %d]", connect->sock.fd);
                if (connect->read() && !connect->request()) {
                    connects.erase(connect->sock.fd);
                }
            }
            else if (event.events & EPOLLOUT) {
                log("[write %d]", connect->sock.fd);
                connect->write();
            }
        }
    }

    close(epfd);
    return 0;
}