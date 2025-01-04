cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 11)
add_library(libinput input.c)

target_link_libraries(libinput PUBLIC libshared)

target_compile_options(libinput PRIVATE "-g" "-O3" "-std=c11")
