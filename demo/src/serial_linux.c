#include "ccli/serial.h"
#include <string.h>
#include <pty.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

static struct
{
    byte_sent_func send_next_byte;
    byte_received_func receive_byte;
    int master, slave;

    int write_fifo[2];

    pthread_t read_thread;
    pthread_t write_thread;
    volatile int exit_threads;
} c;

static void* read_thread(void* args)
{
    char b;
    while (!c.exit_threads)
    {
        read(c.master, &b, 1);
        c.receive_byte(b);
    }
    pthread_exit(NULL);
}

static void* write_thread(void* args)
{
    char b;
    while (!c.exit_threads)
    {
        read(c.write_fifo[0], &b, 1);
        write(c.master, &b, 1);
        c.send_next_byte();
    }
    pthread_exit(NULL);
}

static int init(byte_sent_func byte_sent, byte_received_func byte_received)
{
    c.send_next_byte = byte_sent;
    c.receive_byte = byte_received;
    return 1;
}

static void deinit(void)
{
    memset(&c, 0, sizeof c);
}

static int start(void)
{
    if (openpty(&c.master, &c.slave, NULL, NULL, NULL) != 0)
        return 0;

    printf("%s\n", ttyname(c.slave));

    pipe(c.write_fifo);

    c.exit_threads = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&c.read_thread, &attr, read_thread, NULL);
    pthread_create(&c.read_thread, &attr, write_thread, NULL);
    pthread_attr_destroy(&attr);

    return 1;
}

static void stop(void)
{
    c.exit_threads = 1;

    char b = '\0';
    write(c.slave, &b, 1);
    write(c.write_fifo[1], &b, 1);

    void* ret;
    pthread_join(c.read_thread, &ret);

    close(c.master);
    close(c.slave);

    close(c.write_fifo[0]);
    close(c.write_fifo[1]);
}

static int send_byte(char b)
{
    return write(c.write_fifo[1], &b, 1);
}

static int can_send_byte(void)
{
    return 1;
}

struct serial_interface_t serial_linux = {
    .init = init,
    .deinit = deinit,
    .start = start,
    .stop = stop,
    .send_byte = send_byte,
    .can_send_byte = can_send_byte
};
