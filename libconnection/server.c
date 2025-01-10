#include "server.h"
#include "connection.h"

_Bool id_predicate(void* item, void* data) {
    return ((connection*)item)->id == *(int*)data;
}

void send_data(server* server, int connection_id, void* data, size_t size) {
    pthread_mutex_lock(&server->mutex);
    connection* con = sll_find(&server->connections, id_predicate, &connection_id);
    if (con != NULL) con->send(con, data, size);
    pthread_mutex_unlock(&server->mutex);
}

void server_close_connection(server* server, int connection_id) {
    pthread_mutex_lock(&server->mutex);
    connection* con = sll_find(&server->connections, id_predicate, &connection_id);
    if (con != NULL) {
        con->close(con);
        // n*n slow find, should be good enough
        int len = sll_get_size(&server->connections);
        for (int i = 0 ; i < len; i++) {
            connection* c = sll_get_ref(&server->connections, i);
            if (c->id == connection_id) {
                sll_remove(&server->connections, i);
                break;
            }
        }
    }
    pthread_mutex_lock(&server->mutex);
}

void server_destroy(server* server) {
    server->free_func(server);
    pthread_mutex_destroy(&server->mutex);
}