cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 11)
add_library(libgraphics graphics.c)

target_link_libraries(libgraphics PUBLIC libshared)

target_compile_options(libgraphics PRIVATE "-g" "-O3" "-std=c11")

