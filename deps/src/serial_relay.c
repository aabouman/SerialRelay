#include <assert.h>

#include "serial_relay.h"

#define PORT_BUFFER_SIZE 1024

// printf("Line #: %d.\n", __LINE__);

// Open a serial port in read write mode and the associated ZMQ ports.
// If any issue arises closes everything and returns a NULL pointer.
serial_zmq_relay *open_relay(const char *port_name,
                             int baudrate,
                             const char *sub_endpoint,
                             const char *pub_endpoint)
{
    // Initialize all the allocated variables
    struct sp_port *port;
    void *context;
    void *serial_subscriber_socket;
    uint8_t *msg_sub_buffer;
    void *serial_publisher_socket;
    uint8_t *msg_pub_buffer;

    printf("Line #: %d.\n", __LINE__);

    // Allocate memory for relay object
    serial_zmq_relay *relay = calloc(1, sizeof(serial_zmq_relay));
    if (msg_sub_buffer == NULL) { return NULL; }
    // Fill in the relay object
    relay->port                     = port;
    relay->context                  = context;
    relay->serial_subscriber_socket = serial_subscriber_socket;
    relay->msg_sub_buffer           = msg_sub_buffer;
    relay->serial_publisher_socket  = serial_publisher_socket;
    relay->msg_pub_buffer           = msg_pub_buffer;

    printf("Line #: %d.\n", __LINE__);
    printf("port pointer: %p\n", relay->port);

    // Result check ints
    enum sp_return pc;
	int rc = 0;
	int conflate = 1;

    // ***************** Initialize Pointers *****************
    // Setup the serial ports
    pc = sp_get_port_by_name(port_name, &port);
    if(pc != SP_OK) { goto fail; }

    pc = sp_open(port, SP_MODE_READ);
	if(pc != SP_OK) { goto fail; }

    pc = sp_set_baudrate(port, baudrate);
	if(pc != SP_OK) { goto fail; }

    printf("Line #: %d.\n", __LINE__);

    // Setup the message buffers
    msg_sub_buffer = (uint8_t *) calloc( PORT_BUFFER_SIZE, sizeof(uint8_t) );
    if(msg_sub_buffer == NULL) { goto fail; }

    msg_pub_buffer = (uint8_t *) calloc( PORT_BUFFER_SIZE, sizeof(uint8_t) );
	if(msg_pub_buffer == NULL) { goto fail; }

    printf("Line #: %d.\n", __LINE__);

    // Setup the zmq ports
	context = zmq_ctx_new();
    if(context == NULL) { goto fail; }

	serial_publisher_socket = zmq_socket(context, ZMQ_PUB);
    if(serial_publisher_socket == NULL) { goto fail; }

	serial_subscriber_socket = zmq_socket(context, ZMQ_SUB);
    if(serial_subscriber_socket == NULL) { goto fail; }

    printf("Line #: %d.\n", __LINE__);

    // Set socket options before connecting subsciber to the port
	rc = zmq_setsockopt(serial_subscriber_socket, ZMQ_SUBSCRIBE, "", 0);
	if(rc != 0) { goto fail; }

    // Set conflate option for serial subsciber
	rc = zmq_setsockopt(serial_subscriber_socket, ZMQ_CONFLATE, &conflate, sizeof(conflate));
	if(rc != 0) { goto fail; }

    rc = zmq_connect(serial_subscriber_socket, sub_endpoint);
	if(rc != 0) { goto fail; }

    printf("Line #: %d.\n", __LINE__);

    // Set conflate option for serial publisher
	rc = zmq_setsockopt(serial_publisher_socket, ZMQ_CONFLATE, &conflate, sizeof(conflate));
	if(rc != 0) { goto fail; }

	rc = zmq_bind(serial_publisher_socket, pub_endpoint);
	if(rc != 0) { goto fail; }

    printf("Line #: %d.\n", __LINE__);

    return relay;

fail:
    close_relay(relay);

    return NULL;
}


void relay_read(serial_zmq_relay * relay)
{
    int rc = 0;
    int pc = 0;

    // Check how many bytes are avalible from the serial port and read them in
    int bytes_waiting = sp_input_waiting(relay->port);
    if (bytes_waiting != -1)
    {
        printf("%d\n", bytes_waiting);
    }

    if (bytes_waiting > 0)
    {
        pc = sp_blocking_read(relay->port,
                              (void *)relay->msg_pub_buffer,
                              bytes_waiting,
                              (unsigned int) 1000
                              );
        assert(pc == bytes_waiting);

        for (int i = 0; i < bytes_waiting; i++)
        {
            printf("%d", relay->msg_pub_buffer);
        }
        printf("\n");

        // Relay those bytes through zmq
        rc = zmq_send(relay->serial_publisher_socket,
                      (void *)relay->msg_pub_buffer,
                      bytes_waiting,
                      0);
        assert(rc == bytes_waiting);
    }

    return;
}

void relay_write(serial_zmq_relay * relay)
{
    int pc = 0;

    // Read bytes from ZMQ message
    int nbytes = zmq_recv(relay->serial_subscriber_socket,
                          (void *)relay->msg_sub_buffer,
                          PORT_BUFFER_SIZE,
                          0);
    assert(nbytes != -1);

    if (nbytes > 0)
    {
        pc = sp_nonblocking_write(relay->port,
                                relay->msg_pub_buffer,
                                nbytes
                                );
        assert(pc == nbytes);
    }

    return;
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
    rc = zmq_close(relay->serial_subscriber_socket);
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
