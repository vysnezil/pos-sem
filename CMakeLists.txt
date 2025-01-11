cmake_minimum_required(VERSION 3.31)
project(pos_sem C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(libstructures)
add_subdirectory(libdraw)
add_subdirectory(libinput)
add_subdirectory(libconnection)
add_subdirectory(libshared)

add_subdirectory(client)
add_subdirectory(server)
