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

//TODO poner aca todo el tema de las strings.
//void generate_response(char *buffer)

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
    unsigned char buffer_leer[1024] = {0};
    unsigned char *buffer_ptr = buffer_leer;
    int bytes_read;
    int codons_received = 0;
    while ((bytes_read = recv(new_fd, buffer_ptr, sizeof buffer_leer, 0))>0){
//        printf("se leyeron: %i\n", bytes_read);
        buffer_ptr+=bytes_read;
        codons_received += bytes_read;
    }

//    *buffer_ptr = '\0';

// TODO con el tamanio de lo que se leyo malloc para pasarle al decoder.

    size_t decoded_aminos[1024];

    decode_buffer(buffer_leer, decoded_aminos, codons_received);
    amino_counter_t counter;
    amino_counter_create(&counter);
    amino_counter_process(&counter, decoded_aminos, codons_received);


//    TODO LOOP PARA IR MANDANDO SI ES QUE HAY PRIMERO SEGUNDO ETC. por ahora uso un buffer gigante y listo

    char output[1024];
    char *output_ptr = output;
    int chars_written = 0;
    int output_size = 0;
    size_t amino_count = amino_counter_get_amino_count(&counter);
    chars_written = snprintf(output_ptr, sizeof(output), "Cantidad de proteínas encontradas: %zu\n\nAminoácidos más frecuentes:\n", amino_count);
    output_size += chars_written;


    int primero = amino_counter_get_first(&counter);
    output_ptr += chars_written;
    chars_written = snprintf(output_ptr, sizeof(output), "1) %s: %zu\n", amino_name(primero), amino_counter_get_freq(&counter, primero));
    output_size += chars_written;

    output_ptr += chars_written;
    int segundo = amino_counter_get_second(&counter);
    chars_written = snprintf(output_ptr, sizeof(output), "2) %s: %zu\n", amino_name(segundo), amino_counter_get_freq(&counter, segundo));
    output_size += chars_written;

    output_ptr += chars_written;
    int tercero = amino_counter_get_third(&counter);
    chars_written = snprintf(output_ptr, sizeof(output), "3) %s: %zu\n", amino_name(tercero), amino_counter_get_freq(&counter, tercero));
    output_size += chars_written;
    ++output_size; //incluir el \0

    output_ptr = output;
    int bytes_left, bytes_sent;
    for (bytes_left = output_size; bytes_left>0;) {
        if ((bytes_sent=send(new_fd, output_ptr, bytes_left, 0))<=0) {
            exit(-1);
        } else {
            bytes_left-=bytes_sent;
            output_ptr+=bytes_sent;
//            printf("mando %d bytes\n", bytes_sent);
        }
    }

    shutdown(new_fd, 1); //puede dar error
//    printf("%s", output);



    return 0;
}
