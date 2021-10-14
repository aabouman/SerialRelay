#include <stdio.h>

#include "serial_relay.h"

#define PORT_BUFFER_SIZE 1024

// Macro for callocing a
#define ALLOC_OR_FAIL(arr, nmemb) do {                                  \
    if ((nmemb) > 0) {                                                  \
        (arr) = calloc((nmemb), sizeof(*(arr)));                        \
        if (!(arr))                                                     \
            goto fail;                                                  \
    }                                                                   \
} while (0)


// Open a serial port in read write mode
serial_zmq_relay * open_relay(
                              char * port_name,
                              int baudrate,
                              const char * sub_endpoint,
                              const char * pub_endpoint,
                              )
{
    sp_port * port;
    void * context;
    void * serial_subscriber_socket;
    uint8_t * msg_sub_buffer;
    void * serial_publisher_socket;
    uint8_t * msg_pub_buffer;
    // Create serial port relay object and return
    serial_zmq_relay relay = {
                              port,
                              context,
                              serial_subscriber_socket,
                              msg_sub_buffer
                              serial_publisher_socket,
                              msg_pub_buffer,
                              };

    // Result check ints
    enum sp_return pc;
	int rc = 0;
	int conflate = 1;

    // Setup the serial ports
    pc = sp_get_port_by_name(port_name, &port);
    if(pc != SP_OK) { goto fail; }
    pc = sp_open(port, SP_MODE_READ);
	if(pc != SP_OK) { goto fail; }
    pc = sp_set_baudrate(port, baudrate);
	if(pc != SP_OK) { goto fail; }

    // Setup the message buffers
    msg_sub_buffer = (uint8_t *) calloc( PORT_BUFFER_SIZE, sizeof(uint8_t) );
    if(msg_sub_buffer == NULL) { goto fail; }
    msg_pub_buffer = (uint8_t *) calloc( PORT_BUFFER_SIZE, sizeof(uint8_t) );
	if(msg_pub_buffer == NULL) { goto fail; }

    // Setup the zmq ports
	context = zmq_ctx_new();
    if(context == NULL) { goto fail; }
	serial_publisher_socket = zmq_socket(context, ZMQ_PUB);
    if(serial_publisher_socket == NULL) { goto fail; }
	serial_subscriber_socket = zmq_socket(context, ZMQ_SUB);
    if(serial_subscriber_socket == NULL) { goto fail; }

	// Must set socket options before connecting subsciber to the port
	rc = zmq_setsockopt(serial_subscriber_socket, ZMQ_SUBSCRIBE, "", 0);
	if(rc == 0) { goto fail; }
    // Set conflate option for serial subsciber
	rc = zmq_setsockopt(serial_subscriber_socket, ZMQ_CONFLATE, &conflate, sizeof(conflate));
	if(rc == 0) { goto fail; }
	rc = zmq_connect(serial_subscriber_socket, sub_endpoint);
	if(rc == 0) { goto fail; }

    // Set conflate option for serial publisher
	rc = zmq_setsockopt(serial_publisher_socket, ZMQ_CONFLATE, &conflate, sizeof(conflate));
	if(rc == 0) { goto fail; }
	rc = zmq_bind(serial_publisher_socket, pub_endpoint);
	if(rc == 0) { goto fail; }

    return &relay;

fail:
    close_relay(&relay)
    // sp_close(port);
    // sp_free_port(port);
    // free(msg_sub_buffer);
    // free(msg_pub_buffer);
    // void * context;
    // void * serial_publisher_socket;
    // void * serial_subscriber_socket;

    // zmq_close(relay->serial_subsciber_socket);
    // zmq_close(relay->serial_publisher_socket);
    // zmq_ctx_destroy(relay->context);

    return err;
}

bool func(serial_zmq_relay * relay)
{
    int bytes_waiting = check(sp_input_waiting(relay->port));


}

bool close_relay(serial_zmq_relay * relay)
{
    // Result check int
    enum sp_return pc;
	int rc = 0;

    // Free the buffers
    free(relay->msg_pub_buffer);
    free(relay->msg_sub_buffer);
    // Close the zmq sockets and context
    rc = zmq_close(relay->serial_subsciber_socket);
    assert(rc == 0);
    rc = zmq_close(relay->serial_publisher_socket);
    assert(rc == 0);
    rc = zmq_ctx_destroy(relay->context);
    assert(rc == 0);

    // Close out the serial port
    pc == sp_close(relay->port);
    assert(rc == SP_OK);
    sp_free_port(relay->port);

    return true;
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
