#include <stdio.h>
#include <stdbool.h>

#include "serial_relay.h"


int main()
{
    const char *port_name = "/dev/tty.usbmodem92225501";
    int baudrate = 115200;
    const char *sub_endpoint = "tcp://127.0.0.1:5556";
    const char *pub_endpoint = "tcp://127.0.0.1:5557";

    serial_zmq_relay *relay = open_relay(port_name, baudrate, sub_endpoint, pub_endpoint);

    while (true)
    {
        relay_read(relay);
    }

    close_relay(relay);
}
