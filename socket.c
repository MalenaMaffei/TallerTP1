#define _POSIX_C_SOURCE 200112L

#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket.h"
#define OK 0
#define NOK -1

int socket_create(socket_t* self, struct addrinfo* ptr){
    int skt = 0;
    skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    self->fD = skt;
    return skt;
}

int socket_connect(socket_t* self, struct sockaddr *addr, socklen_t addrlen){
    return connect(self->fD, addr, addrlen);
}

int socket_create_and_connect(socket_t* self, struct addrinfo* addr){
    struct addrinfo* ptr;
    bool connected = false;
    int status = 0;
    int skt = 0;

    for (ptr = addr; ptr != NULL && connected == false; ptr = ptr->ai_next) {
        skt = socket_create(self, ptr);
        if (skt == NOK) {
            return NOK;
        } else {
            status = socket_connect(self, ptr->ai_addr, ptr->ai_addrlen);
            if (status == NOK) {
                close(skt);
                return NOK;
            }
            connected = (status != NOK);
        }
    }

    self->fD = skt;
    return OK;
}

int socket_send(socket_t* self, unsigned char *source, size_t length){
    int bytes_left, bytes_sent;
    unsigned char *buffer_ptr = source;
    for (bytes_left = length; bytes_left>0;) {
        if ((bytes_sent=send(self->fD, buffer_ptr, bytes_left, 0))<=0) {
            return NOK;
        } else {
            bytes_left-=bytes_sent;
            buffer_ptr+=bytes_sent;
        }
    }
    return OK;
}

int socket_bind_and_listen(socket_t* self, struct addrinfo* addr, int backlog){
    int s_bind = bind(self->fD, addr->ai_addr, addr->ai_addrlen);
    int s_lis = listen(self->fD, backlog);
    if (s_bind < 0 || s_lis <0){return NOK;}
    return OK;
}

 int socket_accept(socket_t* self,socket_t* n,struct sockaddr *a,socklen_t *l){
     int new_fd = accept(self->fD, a, l);
     n->fD = new_fd;
     if (new_fd < 0){ return NOK; }
     return OK;
 }

int socket_receive(socket_t* self, unsigned char *buffer, size_t length){
    int bytes_read = recv(self->fD, buffer, length, MSG_NOSIGNAL);
    return bytes_read;
}

int socket_shutdown(socket_t* self, int mode){
    return shutdown(self->fD, mode);
}

int socket_destroy(socket_t* self){
    return close(self->fD);
}
