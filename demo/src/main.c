#include "ccli/serial.h"

extern struct serial_interface_t serial_linux;

volatile int do_exit = 0;

void byte_sent(void)
{

}

void byte_received(char b)
{
    serial_linux.send_byte(b);
    if (b == ' ')
        do_exit = 1;
}

int main(int argc, char** argv)
{
    serial_linux.init(byte_sent, byte_received);
    serial_linux.start();

    while(!do_exit) {}

    serial_linux.stop();
    serial_linux.deinit();

    return 0;
}
