#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include "../libstructures/syn_buffer.h"

void handle_command(int con_id, void* data, size_t len, syn_buffer* buff);

#endif