#include "DigitalOut.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"
#include "IM920.h"
#include <cstdio>
#include <cstring>

IM920 im920(D5, D4, 19200);
DigitalOut switcher(D0);
DigitalOut led(LED1);

int main()
{
    char buffer[IM920_BUFFER_LIMIT_SIZE];
    bool state = false;
    switcher.write(0);
    led.write(1);
    ThisThread::sleep_for(1s);
    while (true) {
        memset(buffer, 0, IM920_BUFFER_LIMIT_SIZE);
        im920.read(buffer);
        for (int i = 0; i < IM920_BUFFER_LIMIT_SIZE; i++) {
            if (buffer[i] == ':'){
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
        led = state;
        switcher = state;
        ThisThread::sleep_for(10ms);
        printf("line : %s", buffer);
    }
}

