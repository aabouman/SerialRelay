cmake_minimum_required(VERSION 3.0.0)

include(ExternalProject)  # pull in the `ExternalProject` CMake module


get_filename_component(SERIALPORT_PRE ${CMAKE_CURRENT_BINARY_DIR}/_deps/libserialport ABSOLUTE)
message(STATUS "Serialport-pre ${SERIALPORT_PRE}")

get_filename_component(SERIALPORT_DIR ${SERIALPORT_PRE}/libserialport-src ABSOLUTE)
get_filename_component(SERIALPORT_BIN ${SERIALPORT_PRE}/libserialport-build ABSOLUTE)

set(SERIALPORT_STATIC_LIB ${SERIALPORT_BIN}/libs/libserialport.a)
set(SERIALPORT_INCLUDES ${SERIALPORT_BIN}/include)

ExternalProject_Add(serialport
    PREFIX "${SERIALPORT_PRE}"
    GIT_REPOSITORY "https://github.com/sigrokproject/libserialport"

    SOURCE_DIR "${SERIALPORT_DIR}"
    BINARY_DIR "${SERIALPORT_BIN}"

    CONFIGURE_COMMAND ${SERIALPORT_DIR}/autogen.sh && ${SERIALPORT_DIR}/configure
    BUILD_COMMAND make
    BUILD_BYPRODUCTS ${SERIALPORT_STATIC_LIB}
    INSTALL_COMMAND "" # Dont want sys wide install
)

add_library(libserialport STATIC IMPORTED GLOBAL)
add_dependencies(libserialport serialport)

set_target_properties(serialport PROPERTIES
    IMPORTED_LOCATION ${SERIALPORT_STATIC_LIB}
)
set_target_properties(serialport PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${SERIALPORT_INCLUDES}
)

# add_library(libserialport-static IMPORTED STATIC GLOBAL)
# add_dependencies(libserialport serialport-bootstrap)
# add_dependencies(libserialport-static serialport-bootstrap)

# set_target_properties(libserialport PROPERTIES
#     IMPORTED_LOCATION "${SERIALPORT_LIBS_DIR}/libserialport.so"
#     INTERFACE_INCLUDE_DIRECTORIES "${SERIALPORT_HEADERS_DIR}"
# )
# set_target_properties(libserialport-static PROPERTIES
#     IMPORTED_LOCATION "${SERIALPORT_LIBS_DIR}/libserialport.a"
#     INTERFACE_INCLUDE_DIRECTORIES "${SERIALPORT_HEADERS_DIR}"
# )
