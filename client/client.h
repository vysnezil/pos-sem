#ifndef CLIENT_H
#define CLIENT_H

#include <stdatomic.h>
#include "../libconnection/connection.h"
#include "../libinput/input.h"
#include "graphics/graphics.h"
#include "graphics/object.h"
#include "../libshared/game.h"

typedef struct main_context {
    syn_buffer event_buffer;
    graphics_context graphics;
    object_context objects;
    input_context input;
    connection connection;
    atomic_bool running;
    game game;

    void(*on_receive)(void*, size_t, void*);
} main_context;

#endif