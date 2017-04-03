//
// Created by male on 02/04/17.
//

#include <netdb.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__

typedef struct {
    int fD;
//    struct addrinfo *addr;
} socket_t;

int socket_create(socket_t* self, struct addrinfo* ptr);
int socket_connect(socket_t* self, struct sockaddr *addr, socklen_t addrlen);
int socket_create_and_connect(socket_t* self, struct addrinfo* res);
int socket_bind_and_listen(socket_t* self, struct addrinfo* res, int backlog);
int socket_send(socket_t* self, unsigned char *source, size_t length);
int socket_receive(socket_t* self, unsigned char *buffer);
int socket_shutdown(socket_t* self, int mode);
int socket_destroy(socket_t* self);

#endif //__SOCKET_H__
