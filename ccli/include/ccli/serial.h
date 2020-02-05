#ifndef CCLI_SERIAL_H
#define CCLI_SERIAL_H

#include "ccli/config.h"

C_BEGIN

typedef void (*byte_sent_func)(void);
typedef void (*byte_received_func)(char);

struct serial_interface_t
{
    int (*init)(byte_sent_func, byte_received_func);
    void (*deinit)(void);
    int (*start)(void);
    void (*stop)(void);
    int (*send_byte)(char c);
    int (*can_send_byte)(void);
};

C_END

#endif
