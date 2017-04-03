#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "amino_counter.h"
#include "decoder.h"
#include "socket.h"
#include "server.h"
#include <unistd.h>
#define BACKLOG 10

int write_output(amino_counter_t* counter, char *output, size_t size){
    char *ptr = output;
    int written = 0;
    int output_size = 0;

    const char *prots = "Cantidad de proteínas encontradas: ";
    const char *aminos = "\n\nAminoácidos más frecuentes:\n";

    size_t amino_cnt = amino_counter_get_amino_count(counter);
    written = snprintf(ptr, size - written, "%s%zu", prots,amino_cnt);
    output_size += written;
    ptr += written;

    written =  snprintf(ptr, size - written, "%s", aminos);
    output_size += written;
    ptr += written;

    int amino, freq;
    const char *name;
    for (int i = 1; i < 4; ++i) {
        amino = amino_counter_get_rank(counter, i);
        freq = amino_counter_get_freq(counter, amino);
        name = amino_name(amino);
        if (freq == 0){continue;}
        written = snprintf(ptr, size - written, "%i) %s: %i\n", i, name, freq);
        output_size += written;
        ptr += written;
    }

    return output_size;
}


//int main(int argc, char **argv){
//    char *server_port = argv[1];
void server(const char *server_port){
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints;
    struct addrinfo *res;
    int new_fd;
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
        exit(0);
    }
    // make a socket, bind it, and listen on it:


    socket_t socket;
    socket_create(&socket, res);
    socket_bind_and_listen(&socket, res, BACKLOG);
    // sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // bind(sockfd, res->ai_addr, res->ai_addrlen);
    // listen(sockfd, BACKLOG);

    // now accept an incoming connection:

    addr_size = sizeof their_addr;
    new_fd = accept(socket.fD, (struct sockaddr *)&their_addr, &addr_size);
    freeaddrinfo(res);
    /*---- Send message to the socket of the incoming connection ----*/


//    TODO HACER UN BUFFER DINAMICO ACA TAMBIEN
    unsigned char buffer_leer[1024] = {0};
    unsigned char *buffer_ptr = buffer_leer;
    int bytes_read;
    int codons_received = 0;
    while ((bytes_read = recv(new_fd, buffer_ptr, sizeof buffer_leer, 0))>0){
//        printf("se leyeron: %i\n", bytes_read);
        buffer_ptr+=bytes_read;
        codons_received += bytes_read;
    }

// TODO con el tamanio de lo que se leyo malloc para pasarle al decoder.

    size_t decoded_aminos[1024];

    decode_buffer(buffer_leer, decoded_aminos, codons_received);
    amino_counter_t counter;
    amino_counter_create(&counter);
    amino_counter_process(&counter, decoded_aminos, codons_received);


    char output[1024];

    int output_size = write_output(&counter, output, 1024);

    char *output_ptr = output;
    int bytes_left, bytes_sent;
    for (bytes_left = output_size; bytes_left>0;) {
        if ((bytes_sent=send(new_fd, output_ptr, bytes_left, 0))<=0) {
            exit(-1);
        } else {
            bytes_left-=bytes_sent;
            output_ptr+=bytes_sent;
        }
    }

    shutdown(new_fd, 1); //puede dar error

    close(new_fd);
    socket_destroy(&socket);

    exit(0);
}
