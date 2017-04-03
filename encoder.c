#include "encoder.h"
#include <stdio.h>
#include <stdlib.h>
unsigned char codon_encoder(unsigned char *codon){
    unsigned char binary_codon = 0x00;
    size_t shift = 4;
    for (int i = 0; i < 3; ++i) {
        unsigned char current = codon[i];
        switch (current) {
            case 'A':
                current -= 0x41;
                break;
            case 'U':
                current -= 0x54;
                break;
            case 'G':
                current -= 0x45;
                break;
            case 'C':
                current -= 0x40;
                break;
            default:
                exit(0);
        }

        current <<= shift;
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
