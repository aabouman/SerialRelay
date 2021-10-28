cmake_minimum_required(VERSION 3.0.0)
project(SerialRelay VERSION 0.1 LANGUAGES C)

include(get_serial.cmake)
# include(get_zmq.cmake)

include_directories(${SERIALPORT_DIR})
add_executable(test
    test.c
)
target_link_libraries(test
    PRIVATE
        libserialport
)
# add_dependencies(relay_launch
#     libserialport
# )

# target_include_directories(test
#     PRIVATE
#         ${SERIALPORT_LIB_DIR}
# )

# The relay launch executable
# add_executable(relay_launch
#     serial_relay.h
#     serial_relay.c
#     relay_launch.c
# )
# add_dependencies(relay_launch
#     libserialport
# )

# target_include_directories(relay_launch
#     PRIVATE
#         ${SERIALPORT_DIR}
#         $<BUILD_INTERFACE:${libzmq_SOURCE_DIR}/include>
# )

# target_link_libraries(relay_launch
#     PRIVATE
#         libserialport
#         # libzmq
# )