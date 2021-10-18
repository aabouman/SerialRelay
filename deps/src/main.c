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

    int pc = 0;
    // struct sp_port *port;
    // pc = sp_get_port_by_name(port_name, &port);
    // pc = sp_open(port, SP_MODE_READ);
    // pc = sp_set_baudrate(port, baudrate);

    // pc = sp_get_port_by_name(port_name, &(relay->port));
    // pc = sp_open(relay->port, SP_MODE_READ);
    // pc = sp_set_baudrate(relay->port, baudrate);

    // int bytes_waiting = sp_input_waiting(relay->port);
    // printf("%d\n", bytes_waiting);

    relay_read(relay);
    // relay_read(relay);

    close_relay(relay);

    // pc == sp_close(relay->port);
    // sp_free_port(relay->port);
    // close_relay(relay);

    return 0;
}
