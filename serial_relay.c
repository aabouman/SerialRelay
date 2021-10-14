#include <stdio.h>

#include "serial_relay.h"

#define PORT_BUFFER_SIZE 1024

// Open a serial port in read write mode
serial_port_zmq_relay open_relay(
                                 char * port_name,
                                 int baudrate,
                                //  int bits,
                                //  sp_parity parity,
                                //  int stopbits,
                                //  sp_flowcontrol flowcontrol,
                                //  void *context,
                                 const char * sub_endpoint,
                                 const char * pub_endpoint,
                                 )
{
    sp_port * port;
    serial_check(sp_get_port_by_name(port_name, &port));
    serial_check(sp_open(port, SP_MODE_READ));
    serial_check(sp_set_baudrate(port, baudrate));

    // serial_check(sp_set_bits(port, 8));
    // serial_check(sp_set_parity(port, SP_PARITY_NONE));
    // serial_check(sp_set_stopbits(port, 1));
    // serial_check(sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE));

	void * context = zmq_ctx_new();
	void * serial_publisher_socket = zmq_socket(context, ZMQ_PUB);
	void * serial_subscriber_socket = zmq_socket(context, ZMQ_SUB);

    // Result check int
	int rc = 0;

	// Must set socket options before connecting subsciber to the port
	rc = zmq_setsockopt(serial_subscriber_socket, ZMQ_SUBSCRIBE, "", 0);
	assert(rc == 0);
    // Set conflate option for serial subsciber
	int conflate = 1;
	rc = zmq_setsockopt(serial_subscriber_socket, ZMQ_CONFLATE, &conflate, sizeof(conflate));
	assert(rc == 0);
	rc = zmq_connect(serial_subscriber_socket, sub_endpoint);
	assert(rc == 0);

    // Set conflate option for serial publisher
	rc = zmq_setsockopt(serial_publisher_socket, ZMQ_CONFLATE, &conflate, sizeof(conflate));
	assert(rc == 0);
	rc = zmq_bind(serial_publisher_socket, pub_endpoint);
	assert(rc == 0);

    // uint8_t * msg_buffer = (uint8_t *) calloc( size_t num, size_t size );
    // void* calloc( size_t num, size_t size );

    // Create serial port relay object and return
    serial_port_zmq_relay relay = {
                                   port,
                                   context,
                                   serial_subsciber_socket,
                                   serial_publisher_socket,
                                //    msg_buffer,
                                   };

    return relay;
}

bool close_relay(serial_port_zmq_relay relay)
{
    // Result check int
	int rc = 0;

    // Free the buffer
    // free(relay.msg_buffer);
    // Close the zmq sockets and context
    rc = zmq_close(relay.serial_subsciber_socket);
    assert(rc == 0);
    rc = zmq_close(relay.serial_publisher_socket);
    assert(rc == 0);
    rc = zmq_ctx_destroy(relay.context);
    assert(rc == 0);

    // Close out the serial port
    serial_check(sp_close(relay.port));
    sp_free_port(relay.port);
}

int serial_check(enum sp_return result)
{
    /* For this example we'll just exit on any error by calling abort(). */
    char *error_message;

    switch (result) {
        case SP_ERR_ARG:
            printf("Error: Invalid argument.\n");
            abort();
        case SP_ERR_FAIL:
            error_message = sp_last_error_message();
            printf("Error: Failed: %s\n", error_message);
            sp_free_error_message(error_message);
            abort();
        case SP_ERR_SUPP:
            printf("Error: Not supported.\n");
            abort();
        case SP_ERR_MEM:
            printf("Error: Couldn't allocate memory.\n");
            abort();
        case SP_OK:
        default:
            return result;
    }
}
