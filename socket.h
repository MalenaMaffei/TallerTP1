#include <netdb.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__

#define OK 0
#define NOK -1
#define NO_IP 0
typedef struct {
    int fD;
    struct addrinfo *res;
} socket_t;

//Inicializa un socket con su FD usando la estructura addrinfo
int socket_create(socket_t* self, const char *ip, const char *port, int mode);

//Conecta un socket usando la estructura pasada.
// Devuelve OK o NOK si hay error.
//PRE: el socket fue inicializado usando socket_create
int socket_connect(socket_t* self, struct sockaddr *addr, socklen_t addrlen);

//Crea un socket con su FD usando la estructura addrinfo y luego lo conecta
//Hace la conexión iterando sobre addr
// Devuelve OK o NOK si hay error.
int socket_create_and_connect(socket_t* self, const char *ip,const char *port);

//Hace un bind y luego un listen con el socket pasado.
// Devuelve OK o NOK si hay error.
//PRE: el socket fue inicializado usando socket_create
int socket_bind_and_listen(socket_t* self, int backlog);

//Acepta la conexion que se intenta hacer al puerto en la estructura a
//Crea un nuevo socket para comunicarse con ese puerto.
// Devuelve OK o NOK si hay error.
//PRE: self fue inicializado con socket_create.
//Se llamo a socket_bind_and_listen con self.
int socket_accept(socket_t* self,socket_t* n);

//Envia la cantidad length de bytes de source.
// Devuelve OK o NOK si hay error.
//PRE: length es del largo apropiado.
//self fue inicializado y conectado o creado con socket_accept.
int socket_send(socket_t* self, unsigned char *source, size_t length);

//Recibe hasta length bytes y los guarda en buffer.
// Devuelve bytes recibidos o NOK si hay error.
//PRE: length es del largo apropiado.
//self fue inicializado y conectado o creado con socket_accept.
int socket_receive(socket_t* self, unsigned char *buffer, size_t length);

//Realiza un shutdown en el modo indicado sobre el socket
// Devuelve OK o NOK si hay error.
//PRE: el socket fue inicializado usando socket_create
int socket_shutdown(socket_t* self, int mode);

//Destruye la instancia self liberando sus recursos.
//Cierra el socket y libera a struct addrinfo res.
// PRE: self fue inicializado mediante socket_create
int socket_destroy(socket_t* self);

//Destruye un socket accept liberando sus recursos.
//Cierra el socket.
// PRE: self fue inicializado mediante socket_accept
int socket_accept_destroy(socket_t* self);
#endif //__SOCKET_H__
