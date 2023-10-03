#include "mbed.h"
#include "IM920.h"
#include <cstring>

IM920::IM920(PinName tx, PinName rx, int baud) : _im920(tx, rx, baud) {
    _im920.attach(callback(this, &IM920::recv), UnbufferedSerial::RxIrq);
    _read = true;
}

void IM920::recv() {
    int count = 0;
    char byte = 0;
    memset(_buffer, 0, IM920_BUFFER_LIMIT_SIZE);

    for (int i = 0; i < IM920_BUFFER_LIMIT_SIZE; i++) {
        _im920.read(&_buffer[i], 1);
        if (_buffer[i] == '\n') break;
    }

    _read = false;
}

void IM920::read(char *buf) {
    memcpy(buf, _buffer, IM920_BUFFER_LIMIT_SIZE);
    _read = true;
}

int IM920::write(const char *buf, int size) {
    if (!_im920.writeable()) return 0;

    _im920.write(buf, size);

    return 1;
}

bool IM920::is_read() {
    return _read;
}