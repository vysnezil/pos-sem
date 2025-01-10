#include "connection.h"

void connection_close(connection* con) {
    con->close(con);
}

void connection_send(connection* con, void* data, size_t size) {
    if (con != NULL) con->send(con, data, size);
}