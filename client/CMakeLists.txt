cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 11)
add_executable(client client.c main.c ui.c game/game_handler.c ../libshared/game.c graphics/graphics.c menu/menu.c
        menu/basic_menu.c menu/input_menu.c menu/renderer.c graphics/object.c graphics/objects/circle.c
        menu/message_menu.c menu/list_menu.c)

target_compile_options(client PRIVATE "-g" "-std=c11")

target_link_libraries(client PRIVATE libstructures libdraw libinput libconnection)