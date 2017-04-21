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
#define READ_SHTDWN 1
#define SERVER_MODE 0

int static str_out(amino_counter_t* ctr,decoder_t* dec,
                   unsigned char *out,size_t s){
//    Escribe el output en el array output de tamaño size.
    unsigned char *ptr = out;
    int written = 0;
    int output_size = 0;
    int write = 0;
    const char *prots = "Cantidad de proteínas encontradas: ";
    const char *aminos = "\n\nAminoácidos más frecuentes:\n";

    size_t amino_cnt = amino_counter_get_amino_count(ctr);

    write = s;
    written = snprintf((char*)ptr, write, "%s%zu%s", prots,amino_cnt,aminos);
    output_size += written;
    ptr += written;

    int amino, freq;
    const char *name;
    for (int i = 1; i < 4; ++i) {
        amino = amino_counter_get_rank(ctr, i);
        freq = amino_counter_get_freq(ctr, amino);
        name = amino_name(dec, amino);
        if (freq == 0){break;}
        write = s - written;
        written = snprintf((char*)ptr, write, "%i) %s: %i\n", i, name, freq);
        output_size += written;
        ptr += written;
    }

    return output_size;
}

void static recv_aminos(amino_counter_t *ctr, decoder_t *dcdr,socket_t* socket){
//    PRE: el counter ya fue creado.
//    el socket esta listo para recibir.
    unsigned char buffer_leer[BUFFSIZE] = {0};
    size_t decoded_aminos[BUFFSIZE];
    int read = 1;
    while (read>0){
        read = socket_receive(socket, buffer_leer,BUFFSIZE);
        decode(dcdr, buffer_leer, decoded_aminos, read);
        amino_counter_process(ctr, decoded_aminos, read);
    }
}

int server(const char *server_port){
    socket_t socket;
    socket_create(&socket, 0, server_port, SERVER_MODE);

    socket_bind_and_listen(&socket, BACKLOG);

    socket_t new_socket;
    socket_accept(&socket, &new_socket);

    decoder_t decoder;
    decoder_create(&decoder);
    amino_counter_t counter;
    amino_counter_create(&counter);
    recv_aminos(&counter, &decoder, &new_socket);

    unsigned char output[OUTPUTMAX];

    int output_size = str_out(&counter, &decoder, output, sizeof(output));

    socket_send(&new_socket, output, output_size);

    socket_shutdown(&new_socket, READ_SHTDWN);

    socket_accept_destroy(&new_socket);
    socket_destroy(&socket);

    return 0;
}
