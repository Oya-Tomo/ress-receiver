#include "DigitalOut.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"
#include "IM920.h"
#include <chrono>
#include <cstdio>
#include <cstring>

constexpr chrono::milliseconds PROCESS_RATE{50};
constexpr int failFramesLimit = 60;

IM920 im920(D5, D4, 19200);
DigitalOut switcher(D0);
DigitalOut led(LED1);

int main()
{
    char buffer[IM920_BUFFER_LIMIT_SIZE];
    bool state = false;
    int failFrames = 0;

    switcher.write(0);
    led.write(1);
    ThisThread::sleep_for(1s);

    Timer timer;
    timer.start();
    timer.reset();

    while (true) {
        memset(buffer, 0, IM920_BUFFER_LIMIT_SIZE);
        failFrames = im920.is_read() ? failFrames + 1 : 0;
        im920.read(buffer);
        for (int i = 0; i < IM920_BUFFER_LIMIT_SIZE; i++) {
            if (buffer[i] == ':') {
                if(buffer[i+1] == '1') {
                    state = true;
                    break;
                } else {
                    state = false;
                    break;
                }
            } else if (buffer[i] == 0) {
                break;
            }
        }

        if (failFrames > failFramesLimit) {
            state = false;
        }
        led = state;
        switcher = state;

        while (timer.elapsed_time() < PROCESS_RATE);
        timer.reset();
    }
}

