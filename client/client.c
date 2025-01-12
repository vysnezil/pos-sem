#include "client.h"
#include <stdlib.h>
#include <sys/wait.h>

int start_server(short port, size_t round_time) {
    int child = fork();
    if (child == 0) {
        char str_time[6];
        char str_port[6];
        snprintf(str_time, 6, "%zu", round_time);
        snprintf(str_port, 6, "%d", port);
        execlp("./server", "./server", str_port, str_time, "headless", NULL);
        exit(errno);
    }
    if (child == -1) return errno;
    int status;
    sleep(1);
    waitpid( child, &status, WNOHANG);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 0;
}