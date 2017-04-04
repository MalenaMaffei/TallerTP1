#define _POSIX_C_SOURCE 200112L

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

int socket_create_and_connect(socket_t* self, struct addrinfo* addr){
    struct addrinfo* ptr;
    bool is_connected = false;
    int s = 0;
    int skt = 0;

    for (ptr = addr; ptr != NULL && is_connected == false; ptr = ptr->ai_next) {
        skt = socket_create(self, ptr);
        if (skt == -1) {
            return -1;
        } else {
            s = socket_connect(self, ptr->ai_addr, ptr->ai_addrlen);
            if (s == -1) {
                close(skt);
                return -1;
            }
            is_connected = (s != -1);
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

int socket_bind_and_listen(socket_t* self, struct addrinfo* addr, int backlog){
    int s_bind = bind(self->fD, addr->ai_addr, addr->ai_addrlen);
    int s_lis = listen(self->fD, backlog);
    if (s_bind < 0 || s_lis <0){return -1;}
    return 0;
}
 int socket_accept(socket_t* self,socket_t* n,struct sockaddr *a,socklen_t *l){
     int new_fd = accept(self->fD, a, l);
     n->fD = new_fd;
     if (new_fd < 0){ return -1; }
     return 0;
 }

int socket_receive(socket_t* self, unsigned char *buffer, size_t length){
    int bytes_read = recv(self->fD, buffer, length, 0);
    return bytes_read;
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
