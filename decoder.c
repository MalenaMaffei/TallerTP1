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

    second_base = codon;
    second_base >>= 2;
    second_base = base_code(second_base);


    first_base = codon;
    first_base >>= 4;
    first_base = base_code(first_base);

    size_t amino =  genetic_code[first_base][second_base][third_base];

    return amino;
}
const char *amino_names[20] = {
    "Ácido aspártico", "Ácido glutámico",  "Alanina","Arginina", "Asparagina",
    "Cisteína", "Fenilalanina", "Glicina", "Histidina", "Isoleucina", "Leucina",
    "Lisina", "Metionina", "Prolina", "Serina", "Tirosina", "Treonina",
    "Triptófano", "Valina", "Stop"
};

const char* amino_name(size_t amino_code){
    return amino_names[amino_code];
}

void decode_buffer(unsigned char *buffer, size_t *dest, size_t lenght){
    size_t amino;
    unsigned char codon;
    for (int i = 0; i < lenght; ++i) {
        codon = buffer[i];
        amino = get_amino(codon);
        dest[i] = amino;
    }
}

