#define _POSIX_C_SOURCE 200112L

#include <string.h>
#include <stdio.h>

#include "encoder.h"
#include "socket.h"
#include "client.h"

#define R_BUFFSIZE 300
#define E_BUFFSIZE 100
#define OUTPUTMAX 200
#define CLIENT_MODE 1
long static get_file_size(FILE* fp){
    long file_size = 0;
    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_END) == 0) {
            file_size = ftell(fp);
            if (file_size == -1) { return 0; }
            if (fseek(fp, 0L, SEEK_SET) != 0) { return 0; }
        }
    }
    return file_size;
}

int static send_codons(const char *file, socket_t* socket ){
//  PRE: el socket fue creado y esta listo para mandar
    unsigned char source[R_BUFFSIZE];
    unsigned char encoded_codons[E_BUFFSIZE];

    FILE *fp = fopen(file, "rb");
    long file_size = get_file_size(fp);

    while (file_size > 0){
        size_t block;
        if (file_size < R_BUFFSIZE){
            block = file_size;
        } else {
            block = R_BUFFSIZE;
        }
        if (fread(source, sizeof(char), block, fp) != block){return 0;}
        if (ferror(fp) != 0) { return 0; }
        size_t codons_size = block/3;
        encode_str(source, encoded_codons, codons_size);
        socket_send(socket, encoded_codons, codons_size);
        file_size -= block;
    }
    fclose(fp);
    return 0;
}

void static recv_and_print(socket_t* socket){
//  PRE: el socket fue creado y esta listo para recibir.
    unsigned char buffer_leer[OUTPUTMAX] = {0};
    int read = 1;
    while (read >0){
        read = socket_receive(socket, buffer_leer, sizeof(buffer_leer));
    }
    printf("%s", buffer_leer);
}

int client(const char *ip, const char *port, const char *file){
    int status;

    socket_t client_socket;
    status = socket_create_and_connect(&client_socket, ip, port);
    if (status < 0) { return 0; }

    send_codons(file, &client_socket);

    status = socket_shutdown(&client_socket, 1);
    if (status < 0) { return 0; }

    recv_and_print(&client_socket);

    socket_destroy(&client_socket);
    return 0;
}
