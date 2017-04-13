#include "encoder.h"
#include <stdlib.h>

unsigned char static base_encoder(unsigned char base, int shift){
//  al caracter pasado se le resta el numero correspondiente
//  y luego un shift para dejarlo en la posicion correspondiente
//  dentro del byte que sera el codon codificado.
    switch (base) {
        case 'A':
            base -= 0x41;
            break;
        case 'U':
            base -= 0x54;
            break;
        case 'G':
            base -= 0x45;
            break;
        case 'C':
            base -= 0x40;
            break;
        default:
            return 0;
    }
    base <<= shift;
    return base;
}

unsigned char static codon_encoder(unsigned char *codon){
//Se hacen ors de cada base codificada para dejarlas
// a todas en un mismo byte binary_codon
    unsigned char binary_codon = 0x00;
    size_t shift = 4;
    for (int i = 0; i < 3; ++i) {
        unsigned char current;;
        current = base_encoder(codon[i], shift);
        shift -= 2;
        binary_codon |= current;
    }
    return binary_codon;
}

void encode_str(unsigned char *src, unsigned char *dest, size_t dest_size){
    for (size_t i = 0; i < dest_size; ++i) {
        unsigned char encoded_codon = codon_encoder(src);
        dest[i] = encoded_codon;
        src += 3;
    }
}
