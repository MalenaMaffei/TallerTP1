//
// Created by male on 02/04/17.
//

#include <netdb.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__

typedef struct {
    int file_descriptor;
} socket_t;

int socket_create(socket_t* self, struct addrinfo* ptr);
int socket_send(socket_t* self, unsigned char *source, size_t length);
int socket_receive(socket_t* self, unsigned char *buffer);
int socket_shutdown(socket_t* self, int mode);

#endif //__SOCKET_H__
