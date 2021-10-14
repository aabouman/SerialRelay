#include <stdbool.h>

#include "libserialport.h"
#include "zmq.h"

struct sp_port;

typedef struct _serial_zmq_relay
{
    // Setup port struct
    sp_port * port;

    // ZMQ socket
    void * context;
    // Socket which Mercury will message to (this is subscribing socket)
    void * serial_subscriber_socket;
    // Buffer used for recieving dumping serial port buffer into publisher
    uint8_t * msg_sub_buffer;
    // Socket which Mercury will listen to (this is publishing socket)
    void * serial_publisher_socket;
    // Buffer used for recieving dumping serial port buffer into publisher
    uint8_t * msg_pub_buffer;

} serial_zmq_relay;


serial_zmq_relay * open_relay(
                              char * port_name,
                              int baudrate,
                              const char * sub_endpoint,
                              const char * pub_endpoint,
                              );


bool close_relay(
                 serial_zmq_relay * relay,
                 );
