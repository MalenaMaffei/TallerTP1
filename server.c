#define _POSIX_C_SOURCE 200112L

#include <string.h>
#include <stdio.h>
#include "amino_counter.h"
#include "decoder.h"
#include "socket.h"
#include "server.h"

#define BACKLOG 10
#define BUFFSIZE 300
#define OUTPUTMAX 200

int write_output(amino_counter_t* counter, unsigned char *output, size_t size){
//    Escribe el output en el array output de tamaño size.
    unsigned char *ptr = output;
    int written = 0;
    int output_size = 0;
    int write = 0;
    const char *prots = "Cantidad de proteínas encontradas: ";
    const char *aminos = "\n\nAminoácidos más frecuentes:\n";

    size_t amino_cnt = amino_counter_get_amino_count(counter);

    write = size - written;
    written = snprintf((char*)ptr, write, "%s%zu%s", prots,amino_cnt,aminos);
    output_size += written;
    ptr += written;

    int amino, freq;
    const char *name;
    for (int i = 1; i < 4; ++i) {
        amino = amino_counter_get_rank(counter, i);
        freq = amino_counter_get_freq(counter, amino);
        name = amino_name(amino);
        if (freq == 0){continue;}
        write = size - written;
        written = snprintf((char*)ptr, write, "%i) %s: %i\n", i, name, freq);
        output_size += written;
        ptr += written;
    }

    return output_size;
}

void recv_aminos(amino_counter_t *counter, socket_t* socket){
//    PRE: el counter ya fue creado.
//    el socket esta listo para recibir.
    unsigned char buffer_leer[BUFFSIZE] = {0};
    size_t decoded_aminos[BUFFSIZE];
    int read = 1;
    while (read>0){
        read = socket_receive(socket, buffer_leer,BUFFSIZE);
        decode_buffer(buffer_leer, decoded_aminos, read);
        amino_counter_process(counter, decoded_aminos, read);
    }
}

void server(const char *server_port){
    struct sockaddr_storage c_addr;
    struct addrinfo hints;
    struct addrinfo *res;
    int status;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(0, server_port, &hints, &res);
    if (status != 0) {
        exit(0);
    }

    socket_t socket;
    socket_create(&socket, res);
    socket_bind_and_listen(&socket, res, BACKLOG);

    socklen_t addr_s = sizeof c_addr;
    socket_t new_socket;
    socket_accept(&socket, &new_socket, (struct sockaddr *)&c_addr, &addr_s);
    freeaddrinfo(res);

    amino_counter_t counter;
    amino_counter_create(&counter);
    recv_aminos(&counter, &new_socket);

    unsigned char output[OUTPUTMAX];

    int output_size = write_output(&counter, output, sizeof(output));

    socket_send(&new_socket, output, output_size);

    socket_shutdown(&new_socket, 1);

    socket_destroy(&new_socket);
    socket_destroy(&socket);
}

