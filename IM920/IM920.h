#ifndef IM920_H
#define IM920_H

#include "mbed.h"
#include <cstddef>

constexpr int IM920_BUFFER_LIMIT_SIZE = 256;

class IM920 {
public:
    IM920(PinName tx, PinName rx, int baud);
    void recv();
    void read(char *buf); // please input the buffer pointer that has the size of 256 bytes.
    bool is_read();
    int write(const char *buf, int size);

private:
    UnbufferedSerial _im920;
    char _buffer[256];
    bool _read;
};

#endif