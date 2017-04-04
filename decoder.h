#ifndef __DECODER_H__
#define __DECODER_H__
#include <stdlib.h>

// Decodifica el array buffer y guarda la decodificación en dest.
// Los aminoácidos son representados por un índice 0-19.
// Pre: buffer es una tira valida de aminoácidos codificados.
// dest es del largo suficiente y length es el largo de dest.
void decode_buffer(unsigned char *buffer, size_t *dest, size_t length);

// Devuelve puntero al str con el nombre del aminoácido pasado.
// Devuelve NULL si el código no existe.
const char* amino_name(size_t amino_code);

#endif //__DECODER_H__

