#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "amino_counter.h"
#include "decoder.h"
#define BACKLOG 10

int main(int argc, char **argv){
    char *server_port = argv[1];
//    TODO chequear cantidad correcta de argc 2 en este caso



    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints;
    struct addrinfo *res;
    int sockfd, new_fd;
    int status;
    // !! don't forget your error checking for these calls !!

    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    status = getaddrinfo(0, server_port, &hints, &res);
    if (status != 0) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }
    // make a socket, bind it, and listen on it:

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);

    // now accept an incoming connection:

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    freeaddrinfo(res);
    /*---- Send message to the socket of the incoming connection ----*/


//    TODO HACER UN BUFFER DINAMICO ACA TAMBIEN
    char buffer_leer[1024] = {0};
    char *buffer_ptr = buffer_leer;
    int bytes_read;
    int codons_received = 0;
    while ((bytes_read = recv(new_fd, buffer_ptr, sizeof buffer_leer, 0))>0){
        printf("se leyeron: %i\n", bytes_read);
        buffer_ptr+=bytes_read;
        codons_received += bytes_read;
    }

    *buffer_ptr = '\0';

// TODO con el tamanio de lo que se leyo malloc para pasarle al decoder.

    size_t *decoded_aminos[1024]; 




    int iterador = 0;
    while (buffer_leer[iterador] != '\0'){
        printf("posicion %i: 0x%x\n", iterador, buffer_leer[iterador]);
        ++iterador;
    }






    return 0;
}
