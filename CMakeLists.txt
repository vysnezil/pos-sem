cmake_minimum_required(VERSION 3.31)
project(pos_sem C)

set(CMAKE_C_STANDARD 11)
add_executable(pos_sem main.c)

add_subdirectory(libshared)
add_subdirectory(libgraphics)
add_subdirectory(libinput)

target_link_libraries(pos_sem PRIVATE libgraphics libinput)

target_compile_options(libgraphics PRIVATE "-g" "-O3" "-std=c11")