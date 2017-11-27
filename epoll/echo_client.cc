#include "common/header.h"
#include "common/log.h"
#include "common/net/net_util.h"
#include "common/net/epoll_util.h"

#include "epoll/include/packet.h"

struct Connect {
    static atomic<int> count(0);
    int id;
    Socket sock;
    Packet in;
    Packet out;
    string data;
    int requestCount;

    Connect(Socket _sock, const string& _data, const int _requestCount)
        : id(++count), sock(_sock), data(_data), requestCount(_requestCount) {}
    
    void encode();
    void write();
};

void Connect::encode() {
    out.reset();
    char* const data_out = out.buf + Packet::HeaderSize;
    memcpy(data_out, data.c_str(), data.length());
    out.limit = Packet::HeaderSize + data.length();
    out.setPacketSize();
}

void Connect::write() {
}

int main() {
    string[] datas = { "hello, world", "hey, world", "hi, world"  };

    int epfd = epoll_util::createEpoll();
    if (epfd < 0) {
        return -1;
    }

    vector<unique_ptr<Connect>> connects;
    connects.reserve(10);
    for (auto& data : datas) {
        net_util::Socket client_sock = net_util::newClientSocket("127.0.0.1", 8000);
        if (client_sock.invalid()) {
            return -1;
        }

        connects.push_back(make_unique<Connect>(client_sock, data, connect.size() + 2));
        if (epoll_util::addEvent(epfd, EPOLLIN | EPOLLET, client_sock.fd) < 0) {
            log("add client event fail");
            close(epfd);
            return -1;
        }
    }
    for (auto& connect : connects) {
        connect.encode();
        connect.write();
    }

    return 0;
}