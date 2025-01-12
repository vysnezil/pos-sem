cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 11)
add_executable(server main.c server_handler.c)

target_compile_options(server PRIVATE "-g" "-std=c11")

target_compile_definitions(server PUBLIC _XOPEN_SOURCE=500)

target_link_libraries(server PRIVATE libshared libconnection libstructures)