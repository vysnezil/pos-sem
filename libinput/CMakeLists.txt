cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 11)
add_library(libinput input.c)

target_compile_options(libinput PRIVATE "-g" "-O3" "-std=c11")

if (NOT DEFINED TB_IMPL)
    add_compile_definitions(TB_IMPL)
    set(TB_IMPL 1 PARENT_SCOPE)
endif()