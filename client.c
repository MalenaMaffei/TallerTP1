//
// Created by male on 25/03/17.
//

/****************** CLIENT CODE ****************/
#define _POSIX_C_SOURCE 200112L
/*
   Includes clasicos de C
*/
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
/*
   Includes mas propios de la familia *nix.
   Funciones como socket, connect, close son propias del
   sistema operativo.
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <netinet/in.h>
#include <stdlib.h>
#include "encoder.h"

int main(int argc, char **argv){
    char *server_ip = argv[1];
    char *server_port = argv[2];
    char *file_name = argv[3];

//    TODO chequear cantidad correcta de argc 4 en este caso


    int status;
    struct addrinfo hints;
    struct addrinfo *res;  // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_INET;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = 0;

// get ready to connect
    status = getaddrinfo(server_ip, server_port, &hints, &res);
    if (status < 0){
        exit(0);
    }
// servinfo now points to a linked list of 1 or more struct addrinfos

// etc.

// [again, you should do error-checking on getaddrinfo(), and walk
// the "res" linked list looking for valid entries instead of just
// assuming the first one is good (like many of these examples do.)
// See the section on client/server for real examples.]

    int s = 0;
    bool is_connected = false;

    struct addrinfo *ptr;

    int skt = 0;

    for (ptr = res; ptr != NULL && is_connected == false; ptr = ptr->ai_next) {
        /* Creamos el socket definiendo la familia (deberia ser AF_INET IPv4),
           el tipo de socket (deberia ser SOCK_STREAM TCP) y el protocolo (0) */
        skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (skt == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            /* Nos conectamos a la fiuba
               ai_addr encapsula la IP y el puerto del server.
               La estructura es automaticamente creada por getaddrinfo */
            s = connect(skt, ptr->ai_addr, ptr->ai_addrlen);
            if (s == -1) {
                printf("Error: %s\n", strerror(errno));
                close(skt);
            }
            is_connected = (s != -1); // nos conectamos?
        }
    }

    /*---- Read the message from the server into the buffer ----*/
    freeaddrinfo(res);

    //TODO NULL TERMINATOR NO LO VOY A NECESITAR

    unsigned char *source = NULL;
    FILE *fp = fopen(file_name, "r");
    long bufsize;
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
    size_t encoded_codons_size = bufsize/3;
    unsigned char *encoded_codons = NULL;

    encoded_codons = malloc(sizeof(char) * (encoded_codons_size));

    encode_codon_str(source, encoded_codons, encoded_codons_size);

//    for (int i = 0; i < encoded_codons_size; ++i) {
//        printf("posicion %i: 0x%x\n", i, encoded_codons[i]);
//    }

    int bytes_left, bytes_sent;
    unsigned char *buffer_ptr = encoded_codons;
    for (bytes_left = encoded_codons_size; bytes_left>0;) {
        if ((bytes_sent=send(skt, buffer_ptr, bytes_left, 0))<=0) {
            exit(-1);
        } else {
            bytes_left-=bytes_sent;
            buffer_ptr+=bytes_sent;
//            printf("mando %d bytes\n", bytes_sent);
        }
    }

    shutdown(skt, 1); //puede dar error
    free(source); /* Don't forget to call free() later! */
    free(encoded_codons);

    unsigned char buffer_leer[1024] = {0};
    buffer_ptr = buffer_leer;
    int bytes_read;
    int codons_received = 0;
    while ((bytes_read = recv(skt, buffer_ptr, sizeof buffer_leer, 0))>0){
//        printf("se leyeron: %i\n", bytes_read);
        buffer_ptr+=bytes_read;
        codons_received += bytes_read;
    }

    *buffer_ptr = '\0';

    printf("%s", buffer_leer);

    return 0;
}

