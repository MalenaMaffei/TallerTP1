//
// Created by male on 02/04/17.
//
#define _POSIX_C_SOURCE 200112L
/*
   Includes clasicos de C
*/
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <netinet/in.h>
#include <stdlib.h>
#include "socket.h"

//TODO CAMBIAR LO QUE ES CREATE DE LO DE CONNECT
int socket_create(socket_t* self, struct addrinfo* ptr){
    int skt = 0;
    skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    self->fD = skt;
    return skt;
}

//TODO CAMBIAR FIRMA A CONST CHARS
int socket_connect(socket_t* self, struct sockaddr *addr, socklen_t addrlen){
    int status = connect(self->fD, addr, addrlen);
    return status;
}

int socket_create_and_connect(socket_t* self, struct addrinfo* res){
    struct addrinfo* ptr;
    bool is_connected = false;
    int s = 0;
    int skt = 0;

    for (ptr = res; ptr != NULL && is_connected == false; ptr = ptr->ai_next) {
        skt = socket_create(self, ptr);
        if (skt == -1) {
            return -1;
        } else {
            s = socket_connect(self, ptr->ai_addr, ptr->ai_addrlen);
            if (s == -1) {
                close(skt);
                return -1;
            }
            is_connected = (s != -1); // nos conectamos?
        }
    }

    self->fD = skt;
    return 0;
}

int socket_send(socket_t* self, unsigned char *source, size_t length){
    int bytes_left, bytes_sent;
    unsigned char *buffer_ptr = source;
    for (bytes_left = length; bytes_left>0;) {
        if ((bytes_sent=send(self->fD, buffer_ptr, bytes_left, 0))<=0) {
            return -1;
        } else {
            bytes_left-=bytes_sent;
            buffer_ptr+=bytes_sent;
//            printf("mando %d bytes\n", bytes_sent);
        }
    }
    return 0;
}

int socket_receive(socket_t* self, unsigned char *buffer){
//    TODO CHEQUEAR ERROR Y SACAR PARTE DE LEER HASTA 0
    unsigned char *buffer_ptr = buffer;
    int bytes_read;
    int bytes_received = 0;
    while ((bytes_read = recv(self->fD, buffer_ptr, sizeof buffer, 0))>0){
//        printf("se leyeron: %i\n", bytes_read);
        buffer_ptr+=bytes_read;
        bytes_received += bytes_read;
    }
    return 0;
}

int socket_shutdown(socket_t* self, int mode){
    int ret_value = shutdown(self->fD, mode);
    return ret_value;
}

//TODO CHEQUEAR DEVOLUCION DESTROY
int socket_destroy(socket_t* self){
    close(self->fD);
    return 0;
}
