#define _POSIX_C_SOURCE 200112L

#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <memory.h>
#include <stdio.h>

#include "socket.h"
#define SERVER_MODE 0
#define CLIENT_MODE 0

//#define OK 0
//#define NOK -1

int filladdrinfo(socket_t* self, const char *ip,const char *port, int mode){
    int status = 0;

    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (mode == SERVER_MODE){
        hints.ai_flags = AI_PASSIVE;
    } else{
        hints.ai_flags = 0;
    }

    status = getaddrinfo(ip, port, &hints, &res);
    if (status < 0) { return (NOK); }
    if (!res) { return NOK; }
    self->res = res;
    return OK;
}

int socket_create(socket_t* self, const char *ip, const char *port, int mode){
    int skt = 0;
    filladdrinfo(self, ip, port, mode);
    skt = socket(self->res->ai_family, self->res->ai_socktype,
                 self->res->ai_protocol);
    self->fD = skt;
    return skt;
}

int socket_create_and_connect(socket_t* self, const char *ip, const char *port){
    struct addrinfo* ptr;
    bool connected = false;
    int status = 0;
    int skt = 0;

    filladdrinfo(self, ip, port, CLIENT_MODE);

    for (ptr = self->res; ptr != NULL && connected == false;
         ptr = ptr->ai_next) {
        skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (skt == NOK) {
            return NOK;
        } else {
            self->fD = skt;
            status = connect(self->fD,  ptr->ai_addr, ptr->ai_addrlen);
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
    int bytes_left;
    int bytes_sent = 0;
    unsigned char *buffer_ptr = source;
    for (bytes_left = length; bytes_left>0; bytes_left-=bytes_sent) {
        if ((bytes_sent=send(
            self->fD, buffer_ptr, bytes_left, MSG_NOSIGNAL))<=0) {
            return NOK;
        } else {
            buffer_ptr+=bytes_sent;
        }
    }
    return OK;
}

int socket_bind_and_listen(socket_t* self, int backlog){
    int s_bind = bind(self->fD, self->res->ai_addr, self->res->ai_addrlen);
    int s_lis = listen(self->fD, backlog);
    if (s_bind < 0 || s_lis <0){return NOK;}
    return OK;
}

 int socket_accept(socket_t* self,socket_t* n){
     struct sockaddr_storage c_addr;
     struct sockaddr *a;
     socklen_t *l;
     socklen_t addr_s = sizeof c_addr;

     a = (struct sockaddr *)&c_addr;
     l= &addr_s;

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
    if (mode < 0 || mode > 1){ return NOK; }
    return shutdown(self->fD, mode);
}

int socket_destroy(socket_t* self){
    freeaddrinfo(self->res);
    return close(self->fD);
}

int socket_accept_destroy(socket_t* self){
    return close(self->fD);
}
