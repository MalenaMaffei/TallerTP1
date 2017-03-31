//
// Created by male on 29/03/17.
//

#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>

#define MASK 0xfc

enum Aminoacidos {
    FEN,
    LEU,
    SER,
    TIR,
    CIS,
    TRP,
    PRO,
    HIS,
    GLU,
    ILE,
    MET,
    TRE,
    ASN,
    LYS,
    ARG,
    VAL,
    ALA,
    ASP,
    GLI,
    STP
};
size_t genetic_code[4][4][4] = {{{LYS,ASN,LYS,ASN}, {ILE,ILE,MET,ILE}, {ARG,SER,ARG,SER}, {TRE,TRE,TRE,TRE}},
                                   {{STP,TIR,STP,TIR}, {LEU,FEN,LEU,FEN}, {STP,CIS,TRP,CIS}, {SER,SER,SER,SER}},
                                   {{GLU,ASP,GLU,ASP}, {VAL,VAL,VAL,VAL}, {GLI,GLI,GLI,GLI}, {ALA,ALA,ALA,ALA}},
                                   {{GLU,HIS,GLU,HIS}, {LEU,LEU,LEU,LEU}, {ARG,ARG,ARG,ARG}, {PRO,PRO,PRO,PRO}}};

unsigned char base_code(unsigned char base){
    base |= MASK;
    base ^= MASK;
    return base;
}

int find_amino(unsigned char codon){
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
    return genetic_code[first_base][second_base][third_base];
}

const char *amino_names[20] = {"Fenilalanina", "Leucina","Serina","Tirosina","Cisteína", "Triptófano", "Prolina", "Histidina", "Ácido glutámico", "Isoleucina",
                              "Metionina", "Treonina", "Asparagina", "Lisina", "Arginina", "Valina", "Alanina", "Ácido aspártico", "Glicina", "Stop"};
//amino_names[0] = "Fenilalanina";
//amino_names[1] = "Leucina";
//amino_names[2] = "Serina";
//amino_names[3] = "Tirosina";
//amino_names[4] = "Cisteína";
//amino_names[5] = "Triptófano";
//amino_names[6] = "Prolina";
//amino_names[7] = "Histidina";
//amino_names[8] = "Ácido glutámico";
//amino_names[9] = "Isoleucina";
//amino_names[10] = "Metionina";
//amino_names[11] = "Treonina";
//amino_names[12] = "Asparagina";
//amino_names[13] = "Lisina";
//amino_names[14] = "Arginina";
//amino_names[15] = "Valina";
//amino_names[16] = "Alanina";
//amino_names[17] = "Ácido aspártico";
//amino_names[18] = "Glicina";
//amino_names[19] = "Stop";



void main(){
    size_t amino_histogram[20] = {0};


    printf("UUU es 0: %i con nombre: %s\n", find_amino(0x15), amino_names[find_amino(0x15)]);
    printf("CUU es 1: %i con nombre: %s\n", find_amino(0x35), amino_names[find_amino(0x35)]);
    printf("UCU es 2: %i con nombre: %s\n", find_amino(0x1D), amino_names[find_amino(0x1D)]);
    printf("UCC es 2: %i con nombre: %s\n", find_amino(0x1F), amino_names[find_amino(0x1F)]);



    printf("CGU es 14: %i con nombre: %s\n", find_amino(0x39), amino_names[find_amino(0x39)]);
    printf("GCC es 14: %i con nombre: %s\n", find_amino(0x3B), amino_names[find_amino(0x3B)]);
    printf("AGA es 14: %i con nombre: %s\n", find_amino(0x8), amino_names[find_amino(0x8)]);
    printf("AGG es 14: %i con nombre: %s\n", find_amino(0xA), amino_names[find_amino(0xA)]);

}

