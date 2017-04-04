#define _POSIX_C_SOURCE 200112L

#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netdb.h>

#include <stdlib.h>
#include "encoder.h"
#include "socket.h"
#include "client.h"

#define R_BUFFSIZE 300
#define E_BUFFSIZE 100

long get_file_size(FILE* fp){
    long file_size = 0;
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            file_size = ftell(fp);
            if (file_size == -1) { exit(0); }
            if (fseek(fp, 0L, SEEK_SET) != 0) { exit(0); }
        }
    }
    return file_size;
}


//int main(int argc, char **argv){
//    char *ip = argv[1];
//    char *port = argv[2];
//    char *file = argv[3];

void client(const char *ip, const char *port, const char *file){
    int status;
    struct addrinfo hints;
    struct addrinfo *res;  // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_INET;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = 0;


    status = getaddrinfo(ip, port, &hints, &res);
    if (status < 0) { exit(0); }


    socket_t client_socket;
    status = socket_create_and_connect(&client_socket, res);
    if (status < 0) { exit(0); }

    /*---- Read the message from the server into the buffer ----*/
    freeaddrinfo(res);

    unsigned char source[R_BUFFSIZE];
    unsigned char encoded_codons[E_BUFFSIZE];

    FILE *fp = fopen(file, "r");
    long file_size = get_file_size(fp);


    while (file_size > 0){
        size_t block;
        if (file_size < 300){
            block = file_size;
        } else {
            block = R_BUFFSIZE;
        }
        if (fread(source, sizeof(char), block, fp) != block){exit(0);}
        if (ferror(fp) != 0) {exit(0);}
        size_t codons_size = block/3;
        encode_str(source, encoded_codons, codons_size);
        socket_send(&client_socket, encoded_codons, codons_size);
        file_size -= block;
    }
    fclose(fp);


    status = socket_shutdown(&client_socket, 1);
    if (status < 0) { exit(0); }

    unsigned char buffer_leer[1024] = {0};
    int read = 1;
    while (read >0){
       read = socket_receive(&client_socket, buffer_leer, sizeof(buffer_leer));
    }


    printf("%s", buffer_leer);

    socket_destroy(&client_socket);
}
