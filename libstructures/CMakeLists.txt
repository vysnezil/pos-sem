cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 11)
add_library(libstructures syn_buffer.c sll.c)

target_compile_definitions(libstructures PUBLIC _XOPEN_SOURCE=500)

target_compile_options(libstructures PRIVATE "-g" "-O3" "-std=c11")