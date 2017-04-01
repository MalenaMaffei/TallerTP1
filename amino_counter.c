//
// Created by male on 31/03/17.
//
#include "amino_counter.h"
#include <string.h>
#include <stdio.h>


#define STOP_POS 19


void amino_counter_create(amino_counter_t *self) {
    self->first = 0;
    self->second = 0;
    self->third = 0;
    self->amino_count = 0;
    for(int i = 0; i < 20; i++) self->amino_histogram[i] = 0;
}

void amino_counter_destroy(amino_counter_t *self){
    //nothing to do here
}

size_t amino_counter_get_first(amino_counter_t *self) {
    return self->first;
}

size_t amino_counter_get_second(amino_counter_t *self) {
    return self->second;
}

size_t amino_counter_get_third(amino_counter_t *self) {
    return self->third;
}

size_t amino_counter_get_amino_count(amino_counter_t *self) {
    return self->amino_count;
}

void amino_counter_process(amino_counter_t *self, size_t *amino, size_t length){
    for (int i = 0; i < length; ++i) {
        ++self->amino_histogram[amino[i]];
    }

    self->amino_count = self->amino_histogram[STOP_POS];

//    TODO TRES VECES CHEQUEAR EL MAYOR NESTED LOOP, USAR EL DE ARRIBA PARA SALTEAR EL QUE A FUE ELEGIDO ANTERIORMENTE
//    NO ME TENGO QUE PREOCIPAR POR ALFABETICO PORQUE YA ESTAN ORDENADOS.
    for (int k = 0; k < STOP_POS; ++k) {
        size_t current = self->amino_histogram[k];
        if(current > self->first)
            self->first = k;
        else if (current > self->second)
            self->second = k;
        else if (current > self->third)
            self->third = k;
    }

    for (int j = 0; j < 20; ++j) {
        printf("%i\n", self->amino_histogram[j]);
    }
}

size_t amino_counter_get_freq(amino_counter_t *self, size_t amino_code){
    return self->amino_histogram[amino_code];
}

void main(){
    amino_counter_t counter;
    amino_counter_create(&counter);
    size_t amino[17] = {19,19,0,0,1,1,1,2,2,2,2,2,2,3,4,5,6};
    amino_counter_process(&counter, amino, 17);
    printf("primero: %zu y  su freq: %zu\n", amino_counter_get_first(&counter), amino_counter_get_freq(&counter, amino_counter_get_first(&counter)));
    printf("segundo: %zu y  su freq: %zu\n", amino_counter_get_second(&counter), amino_counter_get_freq(&counter, amino_counter_get_second(&counter)));
    printf("ter: %zu y  su freq: %zu\n", amino_counter_get_third(&counter), amino_counter_get_freq(&counter, amino_counter_get_third(&counter)));
    printf("stops: %zu\n", amino_counter_get_amino_count(&counter));

}