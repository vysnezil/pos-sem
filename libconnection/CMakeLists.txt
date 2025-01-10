cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 11)
add_library(libconnection
        connection.c
        server.c
        socket/socket_connection.c
        socket/socket_server.c)

target_compile_options(libconnection PRIVATE "-g" "-std=c11")
