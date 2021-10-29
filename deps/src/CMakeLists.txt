cmake_minimum_required(VERSION 3.0.0)
project(SerialRelay VERSION 0.1 LANGUAGES C)

include(get_serial.cmake)
include(get_zmq.cmake)

add_executable(relay_launch
    serial_relay.h
    serial_relay.c
    relay_launch.c
)
target_include_directories(relay_launch
    PRIVATE
        ${SERIALPORT_DIR}
        $<BUILD_INTERFACE:${libzmq_SOURCE_DIR}/include>
)
target_link_libraries(relay_launch
    PRIVATE
        libserialport
        libzmq
)
# add_dependencies(relay_launch
#     libserialport
#     libzmq
# )