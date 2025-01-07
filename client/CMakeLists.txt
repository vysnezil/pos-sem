cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 11)
add_executable(client main.c graphics/graphics.c menu/menu.c menu/basic_menu.c
        menu/input_menu.c menu/renderer.c)

target_compile_options(client PRIVATE "-g" "-O3" "-std=c11")

target_link_libraries(client PRIVATE libstructures libdraw libinput)