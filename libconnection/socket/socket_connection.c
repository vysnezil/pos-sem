#include "socket_connection.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "socket_server.h"

void* handle_receive(void* arg) {
    connection* con = arg;
    socket_connection_data* data = con->connection_data;
    int id;
    size_t id_recv_len = recv(data->socket, &id, sizeof(size_t), 0);
    if (id_recv_len <= 0) {
        con->on_receive(NULL, 0, con->context);
        return NULL;
    }
    con->id = id;

    while (1) {
        size_t size;
        size_t size_recv_len = recv(data->socket, &size, sizeof(size_t), 0);
        if (size_recv_len <= 0) break;
        void* recv_data = malloc(size);
        size_t data_recv_len = recv(data->socket, recv_data, size, 0);
        if (data_recv_len <= 0) break;
        con->on_receive(recv_data, size, con->context);
    }
    con->on_receive(NULL, 0, con->context);
    return NULL;
}

void handle_send(connection* con, void* data, size_t size) {
    socket_connection_data* c_data = con->connection_data;
    send(c_data->socket, &size, sizeof(size), 0);
    send(c_data->socket, data, size, 0);
}

void socket_con_free(connection* con) {
    socket_connection_data* data = con->connection_data;
    if (data == NULL) return;
    pthread_cancel(data->thread);
    pthread_join(data->thread, NULL);
    close(data->socket);
    free(con->connection_data);
    con->connection_data = NULL;
}

void socket_connection_init(connection* con, int socket) {
    socket_connection_data* data = malloc(sizeof(socket_connection_data));
    con->connection_data = data;
    con->send = handle_send;
    con->close = socket_con_free;
    data->socket = socket;
    data->active = 1;
}

int socket_connect(connection* con, char* host, int port, void(*on_receive)(void*, size_t, void*), void* context) {
    struct sockaddr_in addr = (struct sockaddr_in){
        AF_INET,
        htons(port),
        inet_addr(host)
    };
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (res) return errno;
    socket_connection_init(con, sock);
    con->context = context;
    con->on_receive = on_receive;
    pthread_create(&((socket_connection_data*)con->connection_data)->thread, NULL, handle_receive, con);
    return 0;
}