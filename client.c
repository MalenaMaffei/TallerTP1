//
// Created by male on 25/03/17.
//

/****************** CLIENT CODE ****************/
#define _POSIX_C_SOURCE 200112L

#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netdb.h>

#include <stdlib.h>
#include "encoder.h"
#include "socket.h"
#include "client.h"
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

    //TODO NULL TERMINATOR NO LO VOY A NECESITAR

    unsigned char *source = NULL;
    FILE *fp = fopen(file, "r");
    long bufsize = 0;
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            /* Allocate our buffer to that size. */
            source = malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            /* Read the entire file into memory. */
            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            if (ferror(fp) != 0) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0';
            }
        }
        fclose(fp);
    }
//    printf("source %s", source);
    size_t encoded_codons_size = bufsize/3;
    unsigned char *encoded_codons = NULL;

    encoded_codons = malloc(sizeof(char) * (encoded_codons_size));

    encode_str(source, encoded_codons, encoded_codons_size);

    socket_send(&client_socket, encoded_codons, encoded_codons_size);

    status = socket_shutdown(&client_socket, 1);
    if (status < 0) { exit(0); }

    free(source); /* Don't forget to call free() later! */
    free(encoded_codons);

    unsigned char buffer_leer[1024] = {0};
    socket_receive(&client_socket, buffer_leer);

    printf("%s", buffer_leer);

    socket_destroy(&client_socket);
    exit(0);
}
