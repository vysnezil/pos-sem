#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../libconnection/socket/socket_server.h"

void on_recv(int con_id, void* data, size_t len, void* context) {
    if (len == 0) {
        printf("%d disconnect!\n", con_id);
        return;
    }
    int recv = *(int*)data;
    printf("recv: %d, %d, %d\n", con_id, recv, (int)len);
    recv++;
    send_data(context, con_id, &recv, sizeof(int));
}

int main(int argc, char** argv) {
    server s;
    printf("%s\n", strerror(socket_server_start(&s, 28565, on_recv, &s)));
    sleep(100000);
}