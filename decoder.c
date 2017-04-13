#include <memory.h>
#include "decoder.h"
#define MASK 0xfc
#define AMINO_Q 20
enum Aminoacidos {
    ASP, GLU, ALA, ARG, ASN, CIS, FEN, GLI, HIS, ILE, LEU, LYS, MET, PRO,
    SER, TIR, TRE, TRP, VAL, STP
};

void decoder_create(decoder_t* self){
    const size_t code[4][4][4] = {
     {{LYS,ASN,LYS,ASN}, {ILE,ILE,MET,ILE}, {ARG,SER,ARG,SER}, {TRE,TRE,TRE,TRE}
     },
     {{STP,TIR,STP,TIR}, {LEU,FEN,LEU,FEN}, {STP,CIS,TRP,CIS}, {SER,SER,SER,SER}
     },
     {{GLU,ASP,GLU,ASP}, {VAL,VAL,VAL,VAL}, {GLI,GLI,GLI,GLI}, {ALA,ALA,ALA,ALA}
     },
     {{GLU,HIS,GLU,HIS}, {LEU,LEU,LEU,LEU}, {ARG,ARG,ARG,ARG}, {PRO,PRO,PRO,PRO}
     }
    };
    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++ i) {
            memcpy(&(self->genetic_code[j][i]),&(code[j][i]),4*sizeof(size_t));
        }
    }
    char *amino_names[20] = {
        "Ácido aspártico", "Ácido glutámico",  "Alanina","Arginina",
        "Asparagina","Cisteína", "Fenilalanina", "Glicina", "Histidina",
        "Isoleucina", "Leucina", "Lisina", "Metionina", "Prolina",
        "Serina", "Tirosina", "Treonina","Triptófano", "Valina", "Stop"
    };
    for (int l = 0; l < AMINO_Q; ++l) {
        memcpy(&(self->amino_names[l]), &(amino_names[l]), AMINO_Q);
    }
}

unsigned char base_code(unsigned char base){
// Con las mascara se pone en 0 a los primeros
//  6 bits del byte
    base |= MASK;
    base ^= MASK;
    return base;
}

size_t get_amino(decoder_t* self, unsigned char codon){
//  se shiftea al codon para cada vez dejar en
//  los ultimos dos bits del byte a la base con la
//  que se va a trabajar
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

    size_t amino =  self->genetic_code[first_base][second_base][third_base];

    return amino;
}

char* amino_name(decoder_t* self, size_t amino_code){
    if (amino_code > AMINO_Q - 1){ return NULL; }
    return self->amino_names[amino_code];
}

void decode(decoder_t* self, unsigned char *buf, size_t *dest, size_t length){
    size_t amino;
    unsigned char codon;
    for (int i = 0; i < length; ++i) {
        codon = buf[i];
        amino = get_amino(self, codon);
        dest[i] = amino;
    }
}
