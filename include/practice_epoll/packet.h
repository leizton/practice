#ifndef _PRACTICE_PACKET_H
#define _PRACTICE_PACKET_H

#include <unistd.h>

struct Packet {
    static const int MaxSize = 1024;
    static const int HeaderSize = 4;
    static const int MaxDataSize = MaxSize - HeaderSize;

    char buf[MaxSize];  // total_size(4B) + user_data( (total_size - 4)B )
    int pos = 0;
    int limit = 0;

    void reset() {
        limit = pos = 0;
    }

    int packetSize() {
        return *(int*)buf;
    }

    int dataSize() {
        return packetSize() - Packet::HeaderSize;
    }

    void setPacketSize() {
        *(int*)buf = limit - pos;
    }

    int read(int fd) {
        // 调用全局的同名函数(read)必须加::来指定, 否则是Packet::(this->Packet::)
        int read_num = ::read(fd, buf + pos, MaxSize - pos);
        if (read_num > 0) {
            pos += read_num;
        }
        return read_num;
    }

    bool isReadComplete() {
        return pos >= HeaderSize && pos == packetSize();
    }

    int write(int fd) {
        // 调用全局的同名函数(write)必须加::来指定
        int write_num = ::write(fd, buf + pos, limit - pos);
        if (write_num > 0) {
            pos += write_num;
        }
        // write_num == 0 表示写缓冲已满
        return write_num;
    }

    bool shouldWrite() {
        return pos < limit;
    }
};

#endif  // _PRACTICE_PACKET_H