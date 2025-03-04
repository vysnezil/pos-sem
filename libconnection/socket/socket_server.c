#include "socket_server.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include "../connection.h"
#include "socket_connection.h"

typedef struct socket_server_data {
    int socket;
} socket_server_data;

typedef struct server_connection {
    server* srv;
    connection* con;
} server_connection;

void* handle_single_connection(void* arg) {
    server_connection svc = *(server_connection*)arg;
    free(arg);
    socket_connection_data* con_data = svc.con->connection_data;
    int* i = malloc(sizeof(int));
    *i = svc.con->id;
    svc.srv->on_receive(svc.con->id, i, 0, svc.srv->context);
    int con_id = svc.con->id;
    while (1) {
        size_t size;
        if (recv(con_data->socket, &size, sizeof(size_t), 0) <= 0) break;
        void* buffer = malloc(size);
        if (recv(con_data->socket, buffer, size, 0) <= 0) break;
        svc.srv->on_receive(con_id, buffer, size, svc.srv->context);
    }
    svc.srv->on_receive(con_id, NULL, 0, svc.srv->context);
    return NULL;
}

_Bool conn_id_predicate(void* item, void* data) {
    return ((connection*)item)->id == *(int*)data;
}

void* handle_new_connections(void* arg) {
    server* srv = arg;
    socket_server_data* sd = srv->server_data;
    while (1) {
        int incoming = accept(sd->socket, NULL, NULL);
        connection* con = malloc(sizeof(connection));

        con->id = srv->last_id++;
        send(incoming, &con->id, sizeof(int), 0);

        server_connection* svc = malloc(sizeof(server_connection));
        svc->srv = srv;

        pthread_mutex_lock(&srv->mutex);
        sll_add(&srv->connections, con);
        connection* copy_con = sll_find(&srv->connections, conn_id_predicate, &con->id);
        socket_connection_init(copy_con, incoming);
        svc->con = copy_con;
        pthread_mutex_unlock(&srv->mutex);
        pthread_create(&((socket_connection_data*)copy_con->connection_data)->thread, NULL, handle_single_connection, svc);
        free(con);
    }
    return NULL;
}

void foreach_connection_close(void* con, void* arg) {
    connection* c = con;
    c->close(c);
}

void socket_server_free(server* this) {
    pthread_cancel(this->thread);
    pthread_join(this->thread, NULL);
    pthread_mutex_lock(&this->mutex);
    sll_for_each(&this->connections, foreach_connection_close, this);
    sll_destroy(&this->connections);
    close(((socket_server_data*)this->server_data)->socket);
    pthread_mutex_unlock(&this->mutex);
    pthread_mutex_destroy(&this->mutex);
    free(this->server_data);
}

int socket_server_start(server* this, int port, void(*on_receive)(int, void*, size_t, void*), void* context) {
    this->free_func = socket_server_free;
    this->context = context;
    this->on_receive = on_receive;
    this->last_id = 0;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = (struct sockaddr_in){
        AF_INET,
        htons(port),
        INADDR_ANY
    };

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt));
    int res = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (res) return res;
    listen(sock, 5);

    pthread_mutex_init(&this->mutex, NULL);
    socket_server_data* d = malloc(sizeof(socket_server_data));
    d->socket = sock;
    this->server_data = d;
    sll_init(&this->connections, sizeof(connection));

    pthread_create(&this->thread, NULL, handle_new_connections, this);
    return 0;
}