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
//        printf("\ndespues de resta: 0x%x\n", current);

        current <<= shift;
//        printf("despues de shift: 0x%x\n", current);
        shift -= 2;

        binary_codon |= current;
//        printf("despues de OR: 0x%x\n", binary_codon);
    }
    printf("OR final: 0x%x\n", binary_codon);
    return binary_codon;
}

void encode_codon_str(unsigned char *source, unsigned char *dest, size_t dest_size){
    //responsabilidad del llamador darme un source que sirva

    for (size_t i = 0; i < dest_size; ++i) {
        unsigned char encoded_codon = codon_encoder(source);
        dest[i] = encoded_codon;
        source += 3;
    }
}
