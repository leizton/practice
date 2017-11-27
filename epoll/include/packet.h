#ifndef _PRACTICE_PACKET_H
#define _PRACTICE_PACKET_H

struct Packet {
    static const int MaxSize = 1024;
    static const int HeaderSize = 4;

    char buf[HeaderSize];  // total_size[4 bytes] + user_data[(total_size - 4) bytes]
    int pos = 0;
    int limit = 0;

    int packetSize() {
        return *(int*)buf;
    }

    void setPacketSize() {
        int size = limit - pos;
    }

    bool isReadComplete() {
        return pos >= HeaderSize && pos == packetSize();
    }

    void reset() {
        limit = pos = 0;
    }
};

#endif  // _PRACTICE_PACKET_H