#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"

int main(int argc, char **argv) {
    if (argc < 2){ return 0;}
    const char *mode = argv[1];

    const char* server_mode = "server";
    const char* client_mode = "client";

    if (strcmp(mode, client_mode) == 0){
        if (argc != 5){ return 0;}
        char *server_ip = argv[2];
        char *server_port = argv[3];
        char *file_name = argv[4];
        client(server_ip, server_port,file_name);
    } else if (strcmp(mode, server_mode) == 0){
        if (argc != 3){ return 0;}
        char *server_port = argv[2];
        server(server_port);
    }
}
