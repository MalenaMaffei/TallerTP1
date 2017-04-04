#ifndef __ENCODER_H__
#define __ENCODER_H__
#include <stdlib.h>

// Codifica el array src y guarda la codificacion en dest.
// Pre: src es una tira valida de aminoácidos.
// dest es del largo suficiente y dest_size es el largo de dest.
void encode_str(unsigned char *src, unsigned char *dest, size_t dest_size);

#endif
