//
// Created by male on 29/03/17.
//

#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>

#define MASK 0xfc

enum Aminoacidos {
    ASP,
    GLU,
    ALA,
    ARG,
    ASN,
    CIS,
    FEN,
    GLI,
    HIS,
    ILE,
    LEU,
    LYS,
    MET,
    PRO,
    SER,
    TIR,
    TRE,
    TRP,
    VAL,
    STP
};
size_t genetic_code[4][4][4] = {
    {
    {LYS,ASN,LYS,ASN}, {ILE,ILE,MET,ILE}, {ARG,SER,ARG,SER}, {TRE,TRE,TRE,TRE}
    },
    {
    {STP,TIR,STP,TIR}, {LEU,FEN,LEU,FEN}, {STP,CIS,TRP,CIS}, {SER,SER,SER,SER}
    },
    {
    {GLU,ASP,GLU,ASP}, {VAL,VAL,VAL,VAL}, {GLI,GLI,GLI,GLI}, {ALA,ALA,ALA,ALA}
    },
    {
    {GLU,HIS,GLU,HIS}, {LEU,LEU,LEU,LEU}, {ARG,ARG,ARG,ARG}, {PRO,PRO,PRO,PRO}
    }
};

// size_t amino_histogram[20] = {0};

unsigned char base_code(unsigned char base){
    base |= MASK;
    base ^= MASK;
    return base;
}

size_t get_amino(unsigned char codon){
    unsigned char first_base, second_base, third_base;

    third_base = codon;
    third_base >>= 0;
    third_base = base_code(third_base);
//    third_base |= MASK;
//    third_base ^= MASK;
//    printf("base 3: 0x%x\n", third_base);

    second_base = codon;
    second_base >>= 2;
    second_base = base_code(second_base);
//    second_base |= MASK;
//    second_base ^= MASK;
//    printf("base 2: 0x%x\n", second_base);

    first_base = codon;
    first_base >>= 4;
    first_base = base_code(first_base);
//    first_base |= MASK;
//    first_base ^= MASK;
//    printf("base 1: 0x%x\n", first_base);
    size_t amino =  genetic_code[first_base][second_base][third_base];

    // amino_histogram[amino]++;
    // printf("que carajos puso uno en %i\n", amino );
    return amino;
}
const char *amino_names[20] = {
    "Ácido glutámico", "Ácido aspártico", "Alanina","Arginina", "Asparagina",
    "Cisteína", "Fenilalanina", "Glicina", "Histidina", "Isoleucina", "Leucina",
    "Lisina", "Metionina", "Prolina", "Serina", "Tirosina", "Treonina",
    "Triptófano", "Valina", "Stop"
};

const char* amino_name(int amino_code){
    return amino_names[amino_code];
}

void decode_buffer(unsigned char *received_buffer, size_t *destination, size_t lenght){
    size_t amino;
    unsigned char codon;
    for (int i = 0; i < lenght; ++i) {
        codon = received_buffer[i];
        amino = get_amino(codon);
        destination[i] = amino;
    }
}

int main(){



//    int code = find_amino(0x15);
//    printf("UUU es 0: %i con nombre: %s\n", code, amino_names[code]);
//
//    code = find_amino(0x35);
//    printf("CUU es 1: %i con nombre: %s\n", code, amino_names[code]);
//
//    code = find_amino(0x1D);
//    printf("UCU es 2: %i con nombre: %s\n", code, amino_names[code]);
//
//    code = find_amino(0x1F);
//    printf("UCC es 2: %i con nombre: %s\n", code, amino_names[code]);
//
//    code = find_amino(0x39);
//    printf("CGU es 14: %i con nombre: %s\n", code, amino_names[code]);
//
//    code = find_amino(0x3B);
//    printf("GCC es 14: %i con nombre: %s\n", code, amino_names[code]);
//
//    code = find_amino(0x8);
//    printf("AGA es 14: %i con nombre: %s\n", code, amino_names[code]);
//
//    code = find_amino(0xA);
//    printf("AGG es 14: %i con nombre: %s\n", code, amino_names[code]);

    return 0;
}
