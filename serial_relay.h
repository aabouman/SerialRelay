#include "libserialport.h"
#include "zmq.h"

struct sp_port;

typedef struct _serial_port_zmq_relay
{
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

} serial_port_zmq_relay;


bool open_relay(
                char * port_name,
                int baudrate,
                // int bits,
                // sp_parity parity,
                // int stopbits,
                // sp_flowcontrol flowcontrol,
                // void *context,
                const char * sub_endpoint,
                const char * pub_endpoint,
                );


bool close_relay(
                 serial_port_zmq_relay relay,
                 );
