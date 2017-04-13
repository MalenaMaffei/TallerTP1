#ifndef __DECODER_H__
#define __DECODER_H__
#include <stdlib.h>
#define AMINO_Q 20
typedef struct {
    char *amino_names[AMINO_Q];
    size_t genetic_code[4][4][4];
} decoder_t;

//crea un nuevo decodificador
void decoder_create(decoder_t* self);

// Decodifica el array buffer y guarda la decodificación en dest.
// Los aminoácidos son representados por un índice 0-19.
// Pre: buffer es una tira valida de aminoácidos codificados.
// dest es del largo suficiente y length es el largo de dest.
// self fue inicializado con decoder_create
void decode(decoder_t* self, unsigned char *buf, size_t *dest, size_t length);

// Devuelve puntero al str con el nombre del aminoácido pasado.
// Devuelve NULL si el código no existe.
// self fue inicializado con decoder_create
char* amino_name(decoder_t* self, size_t amino_code);

#endif //__DECODER_H__
